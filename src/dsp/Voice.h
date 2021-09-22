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

#ifndef SRC_DSP_VOICE_H_
#define SRC_DSP_VOICE_H_

#include "ControlListener.h"

#include "OSCEvent.h"

#include "common.h"

#include <vector>
#include <bits/stdint-uintn.h>

namespace cr42y
{
class WTOscillator;
class OscillatorVoiceData;
class ModulationControls;
class AutomationHandler;
class Automation;

class Voice : public ControlListener
{
public:
	Voice(int n, int midivel, std::vector<WTOscillator*>& oscillators, ModulationControls* modCtrls, AutomationHandler* automationHandler);
	virtual ~Voice();

	int getNote();
	float getVelocity();
	void stopPress();
	bool hasEnded();
	
	void calculate(float* left, float* right, uint32_t samples);
private:
	int note;
	float start;
	float velocity;
	float samplerate;
	float baseFrequency;
	bool pressed_;
	bool ended_;
	
	struct OscillatorData
	{
		int id = 0;
		float* wavetable = nullptr;
		uint32_t wtSize = 0;
		uint32_t wtWidth = 0;
		float volume = 0;
		float pan = 0;
		float phaseShift = 0;
		float detune = 0;
		float noteShift = 0;
		float unisonSpread = 0;
		float unisonDetune = 0;
		float wtPos = 0;
		uint32_t unisonAmount = 0;
		float AM = 0;
		float FM = 0;
		float PM = 0;
		float RM = 0;
		float modFactors[CR42Ynth_OSC_COUNT * 4];
		float phase[CR42Ynth_UNISON_MAX];
		float value = 0;
		bool noise = false;
	};
	
	//float modFactors_[CR42Ynth_OSC_COUNT * CR42Ynth_OSC_COUNT * 4];
	
	OscillatorData oscData_[CR42Ynth_OSC_COUNT];
	uint8_t oscCount_;
	
	struct LFOData
	{
		uint32_t id;
		float* waveform;
		uint32_t wavesize;
		float phase;
		float deltaPhase;
		float value;
	};
	
	struct ENVData
	{
		uint32_t id;
		float* samples;
		uint32_t size;
		float pos;
		float deltaPos;
		float sustain;
		float value;
	};
	
	std::vector<LFOData> lfos_;
	std::vector<ENVData> envelopes_;
	
	struct OscParamModData
	{
		uint32_t volumeMod = 0;
		float volumeMin = 0;
		float volumeRange = 0;
		uint32_t panMod = 0;
		float panMin = 0;
		float panRange = 0;
		uint32_t noteshiftMod = 0;
		float noteshiftMin = 0;
		float noteshiftRange = 0;
		uint32_t unisonDetuneMod = 0;
		float unisonDetuneMin = 0;
		float unisonDetuneRange = 0;
		uint32_t unisonSpreadMod = 0;
		float unisonSpreadMin = 0;
		float unisonSpreadRange = 0;
		uint32_t wtPosMod = 0;
		float wtPosMin = 0;
		float wtPosRange = 0;
		uint32_t phaseShiftMod = 0;
		float phaseShiftMin = 0;
		float phaseShiftRange = 0;
		uint32_t modFactorsMod[CR42Ynth_OSC_COUNT * 4];
		float modFactorsMin[CR42Ynth_OSC_COUNT * 4];
		float modFactorsRange[CR42Ynth_OSC_COUNT * 4];
	};
	
	void calcParamMod(uint32_t data, float min, float range, float& out);
	
	OscParamModData oscModData_[CR42Ynth_OSC_COUNT];
	
	void valueCallback(float val, Control* ctrl) override;
	void minCallback(float min, Control* ctrl) override;
	void maxCallback(float max, Control* ctrl) override;
	void genCallback(uint32_t gen, Control* ctrl) override;
	
	struct OscillatorDataControls
	{
		Control* volume = nullptr;
		Control* pan = nullptr;
		Control* phaseShift = nullptr;
		Control* detune = nullptr;
		Control* noteShift = nullptr;
		Control* unisonSpread = nullptr;
		Control* unisonDetune = nullptr;
		Control* wtPos = nullptr;
		Control* modFactors[CR42Ynth_OSC_COUNT * 4];
	};
	
	OscillatorDataControls dataControls_[CR42Ynth_OSC_COUNT];
	
	uint32_t getAutomationData(Automation* automation);
	uint32_t getAutomationData(uint32_t generator);
};

inline void Voice::calcParamMod(uint32_t data, float min, float range, float& out)
{
	switch (data >> 24)
	{
	case TYPE_LFO:
		out = min + lfos_[data & 0xffffff].value * range;
		break;
	case TYPE_ENV:
		out = min + envelopes_[data & 0xffffff].value * range;
		break;
	case TYPE_EXT:
		//TODO: implement
		break;
	default:
		break;
	}
}

} /* namespace cr42y */

#endif /* SRC_DSP_VOICE_H_ */
