/*******************************************************************************
 * Copyright (c) 2019 krokoschlange and contributors.
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

#include "LFO.h"
#include "../CR42Ynth.h"

namespace cr42y
{

LFO::LFO(float rate, PortCommunicator* comm) :
		samplerate(rate),
		waveform(nullptr), //TODO
		smooth(2, comm),
		global(2, comm),
		sync(2, comm),
		useFrequency(2, comm),
		frequency(1, comm, 1, 0, 0),
		pitchScale(2, comm),
		beatNumerator(2, comm),
		beatDenominator(2, comm),
		globalPos(0)
{
}

LFO::~LFO()
{
}

float LFO::getSample(float* wavePos, float noteFreq)
{
	float freq =
			getUseFrequency() ?
					getFrequency() + (noteFreq * getPitchScale() / 100) :
					(CR42Ynth::getInstance()->getBPM() / 60) / (getBeatNumerator() / getBeatDenominator());
	
	float deltaPos = freq / samplerate;
	
	float out = 0;
	if (waveform)
	{
		float waveSample =
				getGlobal() ? globalPos * waveform->size() :
								*wavePos * waveform->size();
		
		if (getSmooth())
		{
			float waveSample2 = waveSample + 1;
			if (waveSample2 > waveform->size())
			{
				waveSample2 -= waveform->size();
			}
			
			float smpl1 = (*waveform)[(int) waveSample];
			float smpl2 = (*waveform)[(int) waveSample2];
			
			out = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
		}
		else
		{
			out = (*waveform)[(int) waveSample];
		}
	}
	if (getGlobal())
	{
		globalPos += deltaPos;
	}
	else
	{
		*wavePos += deltaPos;
	}
	return out;
}

void LFO::setLFO(std::vector<float>* lfo, float freq)
{
	if (waveform)
	{
		delete waveform;
	}
	waveform = lfo;
	setFrequency(freq);
}

void LFO::setFrequency(float freq)
{
	frequency.setValue(freq);
}

void LFO::setSmooth(bool state)
{
	smooth.setValue(state);
}

void LFO::setGlobal(bool state)
{
	global.setValue(state);
}

void LFO::setSync(bool state)
{
	sync.setValue(state);
}

void LFO::setUseFrequency(bool state)
{
	useFrequency.setValue(state);
}

void LFO::setPitchScale(float scale)
{
	pitchScale.setValue(scale);
}

void LFO::setBeatNumerator(int num)
{
	beatNumerator.setValue(num);
}

void LFO::setBeatDenominator(int den)
{
	beatDenominator.setValue(den);
}

void LFO::setGlobalPos(float pos)
{
	globalPos = pos;
}

void LFO::updateSamplerate(float rate)
{
	samplerate = rate;
}

float LFO::getFrequency()
{
	return frequency.getValue();
}

bool LFO::getSmooth()
{
	return smooth.getValue();
}

bool LFO::getGlobal()
{
	return global.getValue();
}

bool LFO::getSync()
{
	return sync.getValue();
}

bool LFO::getUseFrequency()
{
	return useFrequency.getValue();
}

float LFO::getPitchScale()
{
	return pitchScale.getValue();
}

int LFO::getBeatNumerator()
{
	return beatNumerator.getValue();
}

int LFO::getBeatDenominator()
{
	return beatDenominator.getValue();
}

float LFO::getGlobalPos()
{
	return globalPos;
}

} /* namespace cr42y */
