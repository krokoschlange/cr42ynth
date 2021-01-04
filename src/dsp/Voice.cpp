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

#include <iostream>

#include "Voice.h"
#include "common.h"
#include "OscillatorVoiceData.h"
#include "OscillatorControls.h"
#include "WTOscillator.h"

namespace cr42y
{

Voice::Voice(int n, int midivel, std::vector<WTOscillator*> oscillators) :
		note(n),
		velocity(midivel / 127.),
		start(0),
		pressed_(true)
{
	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		oscData_[i].wavetable = nullptr;
	}
	
	oscCount_ = oscillators.size();
	for (int i = 0; i < oscillators.size(); i++)
	{
		oscData_[i].id = oscillators[i]->getNumber();
		std::vector<std::vector<float>> wt = oscillators[i]->wavetable();
		float* wavetable = new float[wt.size() * wt[0].size()];
		for (int j = 0; j < wt.size(); j++)
		{
			for (int k = 0; k < wt[0].size(); k++)
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
		oscData_[i].wtPos = ctrl.getWTPosCtrl()->getValue() * (oscData_[i].wtSize - 1);
		oscData_[i].unisonAmount = ctrl.getUnisonAmountCtrl()->getValue();
		oscData_[i].AM = 1;
		oscData_[i].FM = 1;
		oscData_[i].PM = 0;
		oscData_[i].RM = 1;
		
		//omfg wtf is this crap
		oscModData_[i].volumeMod = 0;
		oscModData_[i].volumeMin = 0;
		oscModData_[i].volumeRange = 1;
		oscModData_[i].panMod = 0;
		oscModData_[i].panMin = 0;
		oscModData_[i].panRange = 1;
		oscModData_[i].noteshiftMod = 0;
		oscModData_[i].noteshiftMin = 0;
		oscModData_[i].noteshiftRange = 0;
		oscModData_[i].unisonDetuneMod = 0;
		oscModData_[i].unisonDetuneMin = 0;
		oscModData_[i].unisonDetuneRange = 0;
		oscModData_[i].unisonSpreadMod = 0;
		oscModData_[i].unisonSpreadMin = 0;
		oscModData_[i].unisonSpreadRange = 0;
		oscModData_[i].wtPosMod = 0;
		oscModData_[i].wtPosMin = 0;
		oscModData_[i].wtPosRange = 0;
		oscModData_[i].phaseShiftMod = 0;
		oscModData_[i].phaseShiftMin = 0;
		oscModData_[i].phaseShiftRange = 0;
		
		for (int j = 0; j < CR42Ynth_OSC_COUNT * 4; j++)
		{
			oscData_[i].modFactors[j] = 0; //TODO: do something useful instead (also below)
			
			oscModData_[i].modFactorsMod[j] = 0;
			oscModData_[i].modFactorsMin[j] = 0;
			oscModData_[i].modFactorsRange[j] = 0;
		}
		
		for (int j = 0; j < oscData_[i].unisonAmount; j++)
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
		
	baseFrequency = std::exp2((note - 69.f) / 12) * 440;
}

Voice::~Voice()
{
	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		if (oscData_[i].wavetable)
		{
			delete[] oscData_[i].wavetable;
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

void Voice::calculate(float* left, float* right, uint32_t samples)
{
	for (int s = 0; s < samples; s++)
	{
		for (int lfo = 0; lfo < lfos_.size(); lfo++)
		{
			LFOData& data = lfos_[lfo];
			data.phase += data.deltaPhase - floorf(data.phase);
			data.value = data.wavetable[(int) (data.phase * data.wavesize)];
		}
		for (int env = 0; env < envelopes_.size(); env++)
		{
			ENVData& data = envelopes_[env];
			data.pos += data.deltaPos - floorf(data.pos);
			if (pressed_ && data.pos > data.sustain)
			{
				data.pos = data.sustain;
			}
			if (data.pos >= data.size)
			{
				data.pos = data.size - 1;
			}
			data.value = data.samples[(int) data.pos];
		}
		
		for (int osc = 0; osc < oscCount_; osc++)
		{
			OscillatorData& data = oscData_[osc];
			
			oscData_[osc].AM = 1;
			oscData_[osc].FM = 1;
			oscData_[osc].PM = 0;
			oscData_[osc].RM = 1;
			for (int osc2 = oscCount_ - 1; osc2 > 0; osc2--)
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
			calcParamMod(oscModData_[osc].phaseShiftMod, oscModData_[osc].phaseShiftMin, oscModData_[osc].phaseShiftRange, data.wtPos);
			
			float oscValue = 0;
			int wtpos = data.wtPos;
			
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
				
				float panFactor = data.pan;
				float phaseCalc = data.detune + data.noteShift;
				if (data.unisonAmount > 1)
				{
					//TODO: make this more reasonable
					panFactor += -data.unisonSpread + 1 * (data.unisonSpread * 2 / (data.unisonAmount - 1));
					
					//TODO: this as well
					phaseCalc += -data.unisonDetune + unison * (data.unisonDetune * 2 / (data.unisonAmount - 1));
				}
				
				panFactor = (panFactor + 1) * M_PI_4;
				
				oscValue *= data.AM * data.RM;
				
				left[s] += oscValue * data.volume * velocity * sinf(panFactor);
				right[s] += oscValue * data.volume * velocity * cosf(panFactor);
				
				phaseCalc = baseFrequency * std::exp2(phaseCalc) * data.FM;
				phaseCalc /= samplerate;
				
				data.phase[unison] += phaseCalc - floorf(data.phase[unison]);
				
			}
			//use unison#0 as value
			data.value = oscValue;
		}
		
		//TODO: update lfo, env
	}
}

void Voice::handleOSCEvent(OSCEvent& event)
{
	
}

} /* namespace cr42y */
