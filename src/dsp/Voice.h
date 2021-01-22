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
class LFO;
	
class Voice : public ControlListener
{
public:
	Voice(int n, int midivel, std::vector<WTOscillator*>& oscillators, ModulationControls* modCtrls, std::vector<LFO*>& lfos);
	virtual ~Voice();

	int getNote();
	float getVelocity();
	
	void calculate(float* left, float* right, uint32_t samples);
private:
	int note;
	float start;
	float velocity;
	float samplerate;
	float baseFrequency;
	bool pressed_;
	
	struct OscillatorData
	{
		int id;
		float* wavetable;
		uint32_t wtSize;
		uint32_t wtWidth;
		float volume;
		float pan;
		float phaseShift;
		float detune;
		float noteShift;
		float unisonSpread;
		float unisonDetune;
		float wtPos;
		uint32_t unisonAmount;
		float AM;
		float FM;
		float PM;
		float RM;
		float modFactors[CR42Ynth_OSC_COUNT * 4];
		float phase[CR42Ynth_UNISON_MAX];
		float value;
		bool noise;
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
		uint32_t sustain;
		float value;
	};
	
	std::vector<LFOData> lfos_;
	std::vector<ENVData> envelopes_;
	
	struct OscParamModData
	{
		uint32_t volumeMod;
		float volumeMin;
		float volumeRange;
		uint32_t panMod;
		float panMin;
		float panRange;
		uint32_t noteshiftMod;
		float noteshiftMin;
		float noteshiftRange;
		uint32_t unisonDetuneMod;
		float unisonDetuneMin;
		float unisonDetuneRange;
		uint32_t unisonSpreadMod;
		float unisonSpreadMin;
		float unisonSpreadRange;
		uint32_t wtPosMod;
		float wtPosMin;
		float wtPosRange;
		uint32_t phaseShiftMod;
		float phaseShiftMin;
		float phaseShiftRange;
		uint32_t modFactorsMod[CR42Ynth_OSC_COUNT * 4];
		float modFactorsMin[CR42Ynth_OSC_COUNT * 4];
		float modFactorsRange[CR42Ynth_OSC_COUNT * 4];
	};
	
	void calcParamMod(uint32_t data, float min, float range, float& out);
	
	OscParamModData oscModData_[CR42Ynth_OSC_COUNT];
	
	enum ParamModType : uint8_t
	{
		TYPE_NONE = 0,
		TYPE_LFO = 1,
		TYPE_ENV = 2,
		TYPE_EXT = 3
	};
	
	void valueCallback(float val, Control* ctrl);
	void minCallback(float min, Control* ctrl);
	void maxCallback(float max, Control* ctrl);
	void genCallback(std::string gen, Control* ctrl);
	
	struct OscillatorDataControls
	{
		Control* volume;
		Control* pan;
		Control* phaseShift;
		Control* detune;
		Control* noteShift;
		Control* unisonSpread;
		Control* unisonDetune;
		Control* wtPos;
		Control* modFactors[CR42Ynth_OSC_COUNT * 4];
	};
	
	OscillatorDataControls dataControls_[CR42Ynth_OSC_COUNT];
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
