/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 * 3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior
 *     written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/


#include <cmath>

#include "Voice.h"
#include "common.h"
#include "OscillatorVoiceData.h"
#include "OscillatorControls.h"
#include "ModulationControls.h"
#include "WTOscillator.h"
#include "AutomationHandler.h"
#include "Automation.h"
#include "common.h"

namespace cr42y
{

Voice::Voice(int n, int midivel, std::vector<WTOscillator*>& oscillators, ModulationControls* modCtrls, AutomationHandler* automationHandler) :
		note(n),
		start(0),
		velocity(midivel / 127.),
		pressed_(true),
		ended_(false)
{
	std::vector<Automation*>& automations = automationHandler->getAutomations();
	for (size_t i = 0; i < automations.size(); i++)
	{
		Automation* automation = automations[i];
		if (automation)
		{
			if (automation->type() == 0)
			{
				ENVData env = {
					automation->id(),
					automation->waveform(),
					(uint32_t) automation->wfSize(),
					0,
					automation->deltaPhase(),
					automation->sustain(),
					0
				};
				envelopes_.push_back(env);
			}
			else
			{
				LFOData lfo = {
					automation->id(),
					automation->waveform(),
					(uint32_t) automation->wfSize(),
					0,
					automation->deltaPhase(),
					0
				};
				lfos_.push_back(lfo);
			}
		}
	}
	
	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		oscData_[i].wavetable = nullptr;
	}
	
	oscCount_ = oscillators.size();
	size_t i = 0;
	for (; i < oscillators.size(); i++)
	{
		oscData_[i].id = oscillators[i]->getNumber();
		std::vector<std::vector<float>> wt = oscillators[i]->wavetable();
		float* wavetable = new float[wt.size() * wt[0].size()];
		for (size_t j = 0; j < wt.size(); j++)
		{
			for (size_t k = 0; k < wt[0].size(); k++)
			{
				wavetable[j * wt[0].size() + k] = wt[j][k];
			}
		}
		oscData_[i].wavetable = wavetable;
		oscData_[i].wtSize = wt.size();
		oscData_[i].wtWidth = wt[0].size();
		
		OscillatorControls& ctrl = oscillators[i]->getControls();
		
		oscData_[i].volume = ctrl.getVolumeCtrl()->getValue();
		oscData_[i].pan = ctrl.getPanCtrl()->getValue();
		oscData_[i].phaseShift = ctrl.getPhaseShiftCtrl()->getValue();
		oscData_[i].detune = ctrl.getDetuneCtrl()->getValue();
		oscData_[i].noteShift = ctrl.getNoteShiftCtrl()->getValue();
		oscData_[i].unisonSpread = ctrl.getUnisonSpreadCtrl()->getValue();
		oscData_[i].unisonDetune = ctrl.getUnisonDetuneCtrl()->getValue();
		oscData_[i].wtPos = ctrl.getWTPosCtrl()->getValue();
		oscData_[i].unisonAmount = ctrl.getUnisonAmountCtrl()->getValue();
		oscData_[i].AM = 1;
		oscData_[i].FM = 1;
		oscData_[i].PM = 0;
		oscData_[i].RM = 1;
		
		dataControls_[i].volume = ctrl.getVolumeCtrl();
		dataControls_[i].volume->addListener(this);
		dataControls_[i].pan = ctrl.getPanCtrl();
		dataControls_[i].pan->addListener(this);
		dataControls_[i].phaseShift = ctrl.getPhaseShiftCtrl();
		dataControls_[i].phaseShift->addListener(this);
		dataControls_[i].detune = ctrl.getDetuneCtrl();
		dataControls_[i].detune->addListener(this);
		dataControls_[i].noteShift = ctrl.getNoteShiftCtrl();
		dataControls_[i].noteShift->addListener(this);
		dataControls_[i].unisonSpread = ctrl.getUnisonSpreadCtrl();
		dataControls_[i].unisonSpread->addListener(this);
		dataControls_[i].unisonDetune = ctrl.getUnisonDetuneCtrl();
		dataControls_[i].unisonDetune->addListener(this);
		dataControls_[i].wtPos = ctrl.getWTPosCtrl();
		dataControls_[i].wtPos->addListener(this);
		
		//omfg wtf is this crap
		oscModData_[i].volumeMod = getAutomationData(dataControls_[i].volume->getGenerator());
		oscModData_[i].volumeMin = dataControls_[i].volume->getMin();
		oscModData_[i].volumeRange = dataControls_[i].volume->getMax() - dataControls_[i].volume->getMin();
		oscModData_[i].panMod = getAutomationData(dataControls_[i].pan->getGenerator());
		oscModData_[i].panMin = dataControls_[i].pan->getMin();
		oscModData_[i].panRange = dataControls_[i].pan->getMax() - dataControls_[i].pan->getMin();
		oscModData_[i].noteshiftMod = getAutomationData(dataControls_[i].noteShift->getGenerator());
		oscModData_[i].noteshiftMin = dataControls_[i].noteShift->getMin();
		oscModData_[i].noteshiftRange = dataControls_[i].noteShift->getMax() - dataControls_[i].noteShift->getMin();
		oscModData_[i].unisonDetuneMod = getAutomationData(dataControls_[i].unisonDetune->getGenerator());
		oscModData_[i].unisonDetuneMin = dataControls_[i].unisonDetune->getMin();
		oscModData_[i].unisonDetuneRange = dataControls_[i].unisonDetune->getMax() - dataControls_[i].unisonDetune->getMin();
		oscModData_[i].unisonSpreadMod = getAutomationData(dataControls_[i].unisonSpread->getGenerator());
		oscModData_[i].unisonSpreadMin = dataControls_[i].unisonSpread->getMin();
		oscModData_[i].unisonSpreadRange = dataControls_[i].unisonSpread->getMax() - dataControls_[i].unisonSpread->getMin();
		oscModData_[i].wtPosMod = getAutomationData(dataControls_[i].wtPos->getGenerator());
		oscModData_[i].wtPosMin = dataControls_[i].wtPos->getMin();
		oscModData_[i].wtPosRange = dataControls_[i].wtPos->getMax() - dataControls_[i].wtPos->getMin();
// 		oscModData_[i].phaseShiftMod = getAutomationData(dataControls_[i].phaseShift->getGenerator());
		oscModData_[i].phaseShiftMod = 0;
		oscModData_[i].phaseShiftMin = dataControls_[i].phaseShift->getMin();
		oscModData_[i].phaseShiftRange = dataControls_[i].phaseShift->getMax() - dataControls_[i].phaseShift->getMin();
		
		for (int j = 0; j < CR42Ynth_OSC_COUNT; j++)
		{
			dataControls_[i].modFactors[j] = modCtrls->amControls[j * CR42Ynth_OSC_COUNT + oscData_[i].id];
			dataControls_[i].modFactors[j]->addListener(this);
			oscData_[i].modFactors[j] = dataControls_[i].modFactors[j]->getValue();
			oscModData_[i].modFactorsMod[j] = getAutomationData(dataControls_[i].modFactors[j]->getGenerator());
			oscModData_[i].modFactorsMin[j] = dataControls_[i].modFactors[j]->getMin();
			oscModData_[i].modFactorsRange[j] = dataControls_[i].modFactors[j]->getMax() - oscModData_[i].modFactorsMin[j];
			
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT] = modCtrls->fmControls[j * CR42Ynth_OSC_COUNT + oscData_[i].id];
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT]->addListener(this);
			oscData_[i].modFactors[j + CR42Ynth_OSC_COUNT] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT]->getValue();
			oscModData_[i].modFactorsMod[j + CR42Ynth_OSC_COUNT] = getAutomationData(dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT]->getGenerator());
			oscModData_[i].modFactorsMin[j + CR42Ynth_OSC_COUNT] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT]->getMin();
			oscModData_[i].modFactorsRange[j + CR42Ynth_OSC_COUNT] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT]->getMax() - oscModData_[i].modFactorsMin[j];
			
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2] = modCtrls->pmControls[j * CR42Ynth_OSC_COUNT + oscData_[i].id];
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2]->addListener(this);
			oscData_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2]->getValue();
			oscModData_[i].modFactorsMod[j + CR42Ynth_OSC_COUNT * 2] = getAutomationData(dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2]->getGenerator());
			oscModData_[i].modFactorsMin[j + CR42Ynth_OSC_COUNT * 2] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2]->getMin();
			oscModData_[i].modFactorsRange[j + CR42Ynth_OSC_COUNT * 2] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2]->getMax() - oscModData_[i].modFactorsMin[j];
			
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3] = modCtrls->rmControls[j * CR42Ynth_OSC_COUNT + oscData_[i].id];
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3]->addListener(this);
			oscData_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3]->getValue();
			oscModData_[i].modFactorsMod[j + CR42Ynth_OSC_COUNT * 3] = getAutomationData(dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3]->getGenerator());
			oscModData_[i].modFactorsMin[j + CR42Ynth_OSC_COUNT * 3] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3]->getMin();
			oscModData_[i].modFactorsRange[j + CR42Ynth_OSC_COUNT * 3] = dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3]->getMax() - oscModData_[i].modFactorsMin[j];
		}
		
		for (size_t j = 0; j < oscData_[i].unisonAmount; j++)
		{
			float phase = oscData_[i].phaseShift + ctrl.getPhaseRandCtrl()->getValue() * ((rand() % 200) - 100) / 100.;
			phase = phase >= 0 ? phase : 1 + (phase - (int) phase); //normalize to 0...1
			phase = phase <= 1 ? phase : phase - (int) phase;
			oscData_[i].phase[j] = phase;
		}
		
		oscData_[i].value = 0;
		oscData_[i].noise = ctrl.getNoiseCtrl()->getValue() > 0.5;
		
		samplerate = oscillators[i]->getSamplerate();
	}
	for (; i < CR42Ynth_OSC_COUNT; i++)
	{
		for (int j = 0; j < CR42Ynth_OSC_COUNT; j++)
		{
			dataControls_[i].modFactors[j] = nullptr;
			oscData_[i].modFactors[j] = 0;
			oscModData_[i].modFactorsMod[j] = 0;
			oscModData_[i].modFactorsMin[j] = 0;
			oscModData_[i].modFactorsRange[j] = 0;
			
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT] = nullptr;
			oscData_[i].modFactors[j + CR42Ynth_OSC_COUNT] = 0;
			oscModData_[i].modFactorsMod[j + CR42Ynth_OSC_COUNT] = 0;
			oscModData_[i].modFactorsMin[j + CR42Ynth_OSC_COUNT] = 0;
			oscModData_[i].modFactorsRange[j + CR42Ynth_OSC_COUNT] = 0;
			
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2] = nullptr;
			oscData_[i].modFactors[j + CR42Ynth_OSC_COUNT * 2] = 0;
			oscModData_[i].modFactorsMod[j + CR42Ynth_OSC_COUNT * 2] = 0;
			oscModData_[i].modFactorsMin[j + CR42Ynth_OSC_COUNT * 2] = 0;
			oscModData_[i].modFactorsRange[j + CR42Ynth_OSC_COUNT * 2] = 0;
			
			dataControls_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3] = nullptr;
			oscData_[i].modFactors[j + CR42Ynth_OSC_COUNT * 3] = 0;
			oscModData_[i].modFactorsMod[j + CR42Ynth_OSC_COUNT * 3] = 0;
			oscModData_[i].modFactorsMin[j + CR42Ynth_OSC_COUNT * 3] = 0;
			oscModData_[i].modFactorsRange[j + CR42Ynth_OSC_COUNT * 3] = 0;
		}
	}
	baseFrequency = std::exp2((note - 69.f) / 12) * 440;
}

Voice::~Voice()
{
	for (int osc = 0; osc < oscCount_; osc++)
	{
		if (oscData_[osc].wavetable)
		{
			delete[] oscData_[osc].wavetable;
		}
		
		dataControls_[osc].volume->removeListener(this);
		dataControls_[osc].pan->removeListener(this);
		dataControls_[osc].phaseShift->removeListener(this);
		dataControls_[osc].detune->removeListener(this);
		dataControls_[osc].noteShift->removeListener(this);
		dataControls_[osc].unisonSpread->removeListener(this);
		dataControls_[osc].unisonDetune->removeListener(this);
		dataControls_[osc].wtPos->removeListener(this);
		for (int i = 0; i < CR42Ynth_OSC_COUNT * 4; i++)
		{
			dataControls_[osc].modFactors[i]->removeListener(this);
		}
	}
}

int Voice::getNote()
{
	return note;
}

float Voice::getVelocity()
{
	return velocity;
}

void Voice::stopPress()
{
	pressed_ = false;
	for (size_t i = 0; i < envelopes_.size(); i++)
	{
		ENVData& data = envelopes_[i];
		float oldVal = data.samples[(int) (data.pos * (data.size - 1))];
		data.pos = data.sustain;
		int smpl = (int) (data.pos * (data.size - 1));
		float val = data.samples[smpl];
		while (val > oldVal)
		{
			smpl++;
			val = data.samples[smpl];
		}
		data.pos = (float) smpl / (data.size - 1);
	}
	for (size_t i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		if (oscModData_[i].volumeMod == 0)
		{
			oscData_[i].volume = 0;
		}
	}
}

bool Voice::hasEnded()
{
	return ended_ && !pressed_;
}

void Voice::calculate(float* left, float* right, uint32_t samples)
{
	for (uint32_t s = 0; s < samples; s++)
	{
		for (size_t lfo = 0; lfo < lfos_.size(); lfo++)
		{
			LFOData& data = lfos_[lfo];
			data.phase += data.deltaPhase - floorf(data.phase);
			data.value = data.waveform[(int) (data.phase * (data.wavesize - 1))];
		}
		for (size_t env = 0; env < envelopes_.size(); env++)
		{
			ENVData& data = envelopes_[env];
			data.pos += data.deltaPos;// - floorf(data.pos);
			if (pressed_ && data.pos > data.sustain)
			{
				data.pos = data.sustain;
			}
			if (data.pos >= 1)
			{
				data.pos = 1;
			}
			data.value = data.samples[(int) (data.pos * (data.size - 1))];
		}
		
		for (int osc = 0; osc < oscCount_; osc++)
		{
			OscillatorData& data = oscData_[osc];
			
			oscData_[osc].AM = 1;
			oscData_[osc].FM = 1;
			oscData_[osc].PM = 0;
			oscData_[osc].RM = 1;
			for (int osc2 = oscCount_ - 1; osc2 >= 0; osc2--)
			{
				calcParamMod(oscModData_[osc].modFactorsMod[osc2], oscModData_[osc].modFactorsMin[osc2], oscModData_[osc].modFactorsRange[osc], data.modFactors[oscData_[osc2].id]);
				
				
				oscData_[osc].AM *= 1 + data.modFactors[oscData_[osc2].id] * (oscData_[osc2].value - 1) / 2;
				
				oscData_[osc].FM *= 1 + data.modFactors[oscData_[osc2].id + CR42Ynth_OSC_COUNT] * 4 * oscData_[osc2].value;
				
				oscData_[osc].PM += 1 + data.modFactors[oscData_[osc2].id + CR42Ynth_OSC_COUNT * 2] * oscData_[osc2].value;
				
				oscData_[osc].RM *= 1 + data.modFactors[oscData_[osc2].id + CR42Ynth_OSC_COUNT * 3] * oscData_[osc2].value;
			}
			
			calcParamMod(oscModData_[osc].volumeMod, oscModData_[osc].volumeMin, oscModData_[osc].volumeRange, data.volume);
			calcParamMod(oscModData_[osc].panMod, oscModData_[osc].panMin, oscModData_[osc].panRange, data.pan);
			calcParamMod(oscModData_[osc].noteshiftMod, oscModData_[osc].noteshiftMin, oscModData_[osc].noteshiftRange, data.noteShift);
			calcParamMod(oscModData_[osc].unisonDetuneMod, oscModData_[osc].unisonDetuneMin, oscModData_[osc].unisonDetuneRange, data.unisonDetune);
			calcParamMod(oscModData_[osc].unisonSpreadMod, oscModData_[osc].unisonSpreadMin, oscModData_[osc].unisonSpreadRange, data.unisonSpread);
			calcParamMod(oscModData_[osc].wtPosMod, oscModData_[osc].wtPosMin, oscModData_[osc].wtPosRange, data.wtPos);
			calcParamMod(oscModData_[osc].phaseShiftMod, oscModData_[osc].phaseShiftMin, oscModData_[osc].phaseShiftRange, data.phaseShift);
			
			float oscValue = 0;
			int wtpos = data.wtPos * (oscData_[osc].wtSize - 1) + 0.5;
			
			for (int unison = data.unisonAmount - 1; unison >= 0; unison--)
			{
				int wavepos = (data.phase[unison] + data.phaseShift + data.PM) * (data.wtWidth - 1);
				wavepos %= data.wtWidth - 1;
				
				if (data.noise)
				{
					//TODO: implement white noise
				}
				else
				{
					oscValue = data.wavetable[wtpos * data.wtWidth + wavepos];
				}
				
				float panFactor = data.pan * 2 - 1;
				float phaseCalc = data.detune + data.noteShift * 48 - 24;
				if (data.unisonAmount > 1)
				{
					//TODO: make this more reasonable
					panFactor += -data.unisonSpread + unison * (data.unisonSpread * 2 / (data.unisonAmount - 1));
					
					//TODO: this as well
					phaseCalc += -data.unisonDetune + unison * (data.unisonDetune * 2 / (data.unisonAmount - 1));
				}
				
				panFactor = (panFactor + 1) * M_PI_4;
				
				oscValue *= data.AM * data.RM;
				
				left[s] += oscValue * data.volume * velocity * sinf(panFactor);
				right[s] += oscValue * data.volume * velocity * cosf(panFactor);
				
				phaseCalc = baseFrequency * std::exp2(phaseCalc / 12) * data.FM;
				phaseCalc /= samplerate;
				
				data.phase[unison] += phaseCalc - floorf(data.phase[unison]);
				
			}
			//use unison#0 as value
			data.value = oscValue;
		}
	}
	ended_ = true;
	for (size_t i = 0; i < envelopes_.size(); i++)
	{
		if (envelopes_[i].pos < 1)
		{
			ended_ = false;
		}
	}
}

void Voice::valueCallback(float val, Control* ctrl)
{
	for (int osc = 0; osc < CR42Ynth_OSC_COUNT; osc++)
	{
		OscillatorDataControls& ctrls = dataControls_[osc];
		
		if (ctrls.volume == ctrl)
		{
			oscData_[osc].volume = val;
		}
		else if (ctrls.pan == ctrl)
		{
			oscData_[osc].pan = val;
		}
		else if (ctrls.phaseShift == ctrl)
		{
			oscData_[osc].phaseShift = val;
		}
		else if (ctrls.detune == ctrl)
		{
			oscData_[osc].detune = val;
		}
		else if (ctrls.noteShift == ctrl)
		{
			oscData_[osc].noteShift = val;
		}
		else if (ctrls.unisonSpread == ctrl)
		{
			oscData_[osc].unisonSpread = val;
		}
		else if (ctrls.unisonDetune == ctrl)
		{
			oscData_[osc].unisonDetune = val;
		}
		else if (ctrls.wtPos == ctrl)
		{
			oscData_[osc].wtPos = val;
		}
		for (int i = 0; i < CR42Ynth_OSC_COUNT * 4; i++)
		{
			if (ctrls.modFactors[i] == ctrl)
			{
				oscData_[osc].modFactors[i] = val;
			}
		}
	}
}

void Voice::minCallback(float min, Control* ctrl)
{
	for (int osc = 0; osc < CR42Ynth_OSC_COUNT; osc++)
	{
		OscillatorDataControls& ctrls = dataControls_[osc];
		
		if (ctrls.volume == ctrl)
		{
			oscModData_[osc].volumeMin = min;
			oscModData_[osc].volumeRange = dataControls_[osc].volume->getMax() - min;
		}
		else if (ctrls.pan == ctrl)
		{
			oscModData_[osc].panMin = min;
			oscModData_[osc].panRange = dataControls_[osc].pan->getMax() - min;
		}
		else if (ctrls.phaseShift == ctrl)
		{
			oscModData_[osc].phaseShiftMin = min;
			oscModData_[osc].phaseShiftRange = dataControls_[osc].phaseShift->getMax() - min;
		}
		else if (ctrls.noteShift == ctrl)
		{
			oscModData_[osc].noteshiftMin = min;
			oscModData_[osc].noteshiftRange = dataControls_[osc].noteShift->getMax() - min;
		}
		else if (ctrls.unisonSpread == ctrl)
		{
			oscModData_[osc].unisonSpreadMin = min;
			oscModData_[osc].unisonSpreadRange = dataControls_[osc].unisonSpread->getMax() - min;
		}
		else if (ctrls.unisonDetune == ctrl)
		{
			oscModData_[osc].unisonDetuneMin = min;
			oscModData_[osc].unisonDetuneRange = dataControls_[osc].unisonDetune->getMax() - min;
		}
		else if (ctrls.wtPos == ctrl)
		{
			oscModData_[osc].wtPosMin = min;
			oscModData_[osc].wtPosRange = dataControls_[osc].wtPos->getMax() - min;
		}
		for (int i = 0; i < CR42Ynth_OSC_COUNT * 4; i++)
		{
			if (ctrls.modFactors[i] == ctrl)
			{
				oscModData_[osc].modFactorsMin[i] = min;
				oscModData_[osc].modFactorsRange[i] = dataControls_[osc].modFactors[i]->getMax() - min;
			}
		}
	}
}

void Voice::maxCallback(float max, Control* ctrl)
{
	for (int osc = 0; osc < CR42Ynth_OSC_COUNT; osc++)
	{
		OscillatorDataControls& ctrls = dataControls_[osc];
		
		if (ctrls.volume == ctrl)
		{
			oscModData_[osc].volumeRange = max - oscModData_[osc].volumeMin;
		}
		else if (ctrls.pan == ctrl)
		{
			oscModData_[osc].panRange = max - oscModData_[osc].panMin;
		}
		else if (ctrls.phaseShift == ctrl)
		{
			oscModData_[osc].phaseShiftRange = max - oscModData_[osc].phaseShiftMin;
		}
		else if (ctrls.noteShift == ctrl)
		{
			oscModData_[osc].noteshiftRange = max - oscModData_[osc].noteshiftMin;
		}
		else if (ctrls.unisonSpread == ctrl)
		{
			oscModData_[osc].unisonSpreadRange = max - oscModData_[osc].unisonSpreadMin;
		}
		else if (ctrls.unisonDetune == ctrl)
		{
			oscModData_[osc].unisonDetuneRange = max - oscModData_[osc].unisonDetuneMin;
		}
		else if (ctrls.wtPos == ctrl)
		{
			oscModData_[osc].wtPosRange = max - oscModData_[osc].wtPosMin;
		}
		for (int i = 0; i < CR42Ynth_OSC_COUNT * 4; i++)
		{
			if (ctrls.modFactors[i] == ctrl)
			{
				oscModData_[osc].modFactorsRange[i] = max - oscModData_[osc].modFactorsMin[i];
			}
		}
	}
}

void Voice::genCallback(uint32_t, Control*)
{
	//haven't thought about this yet, maybe ignore?
}

uint32_t Voice::getAutomationData(Automation* automation)
{
	uint32_t data = 0;
	if (automation->type() == 0)
	{
		data = data | (((uint32_t) TYPE_ENV) << 24);
		for (size_t i = 0; i < envelopes_.size(); i++)
		{
			if (envelopes_[i].id == automation->id())
			{
				data = data | (0xffffff & i); 
				return data;
			}
		}
	}
	else
	{
		data = data | (((uint32_t) TYPE_LFO) << 24);
		for (size_t i = 0; i < lfos_.size(); i++)
		{
			if (lfos_[i].id == automation->id())
			{
				data = data | (0xffffff & i); 
				return data;
			}
		}
	}
	return data;
}

uint32_t Voice::getAutomationData(uint32_t generator)
{
	/*uint32_t data = 0;
	uint8_t type = (generator >> 24);
	uint32_t id = generator & 0xffffff;
	if (type == TYPE_ENV)
	{
		for (size_t i = 0; i < envelopes_.size(); i++)
		{
			if (envelopes_[i].id == id)
			{
				data = ((uint32_t) TYPE_ENV) << 24;
				data = data | (0xffffff & i); 
				return data;
			}
		}
	}
	if (type == TYPE_LFO)
	{
		for (size_t i = 0; i < lfos_.size(); i++)
		{
			if (lfos_[i].id == id)
			{
				data = ((uint32_t) TYPE_LFO) << 24;
				data = data | (0xffffff & i); 
				return data;
			}
		}
	}*/
	uint32_t data = 0;
	uint32_t id = generator & 0xffffff;
	uint8_t type = (generator >> 24);
	if (type == TYPE_LFO || type == TYPE_ENV)
	{
		for (size_t i = 0; i < envelopes_.size(); i++)
		{
			if (envelopes_[i].id == id)
			{
				data = ((uint32_t) TYPE_ENV) << 24;
				data = data | (0xffffff & i); 
				return data;
			}
		}
		for (size_t i = 0; i < lfos_.size(); i++)
		{
			if (lfos_[i].id == id)
			{
				data = ((uint32_t) TYPE_LFO) << 24;
				data = data | (0xffffff & i); 
				return data;
			}
		}
	}
	return data;
}

} /* namespace cr42y */
