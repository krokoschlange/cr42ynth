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

#include "WTOscillator.h"

#include <cmath>

namespace cr42y
{

WTOscillator::WTOscillator(float rate, int id) :
		ID(id),
		smooth(false),
		samplerate(rate),
		wavetable(nullptr),
		detune(0),
		enabled(false)
{
}

WTOscillator::~WTOscillator()
{
	delete wavetable;
}

float WTOscillator::getSample(OscPlayhead* voice)
{
	if (!enabled)
	{
		return 0;
	}
	float out;
	float pos = voice->getPhase();

	int wtSample = voice->getWTPos() * wavetable->size();
	if (wtSample >= wavetable->size())
	{
		wtSample--;
	}
	float waveSample = pos * (*wavetable)[0].size();
	if (waveSample >= (*wavetable)[0].size())
	{
		waveSample -= 1;
	}
	if (smooth)
	{
		float smpl1 = (*wavetable)[wtSample][(int) waveSample];
		float smpl2 = (*wavetable)[wtSample][(int) waveSample + 1];

		out = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
	}
	else
	{
		out = (*wavetable)[wtSample][(int) waveSample];
	}

	float frequency = pow(2, (voice->note + detune - 69) / 12) * 440 * voice->getDeltaFreq() * voice->getFM();
	float deltaPos = frequency / samplerate;//(voice->getFrequency() * detune) / samplerate;
	pos += deltaPos;
	voice->movePhase(pos);

	return out;
}

float WTOscillator::getDetune()
{
	return detune;
}

void WTOscillator::setDetune(float d)
{
	detune = d;
}

void WTOscillator::setDetune(int semi, int cents)
{
	detune = semi + cents / 100;
}

void WTOscillator::setEnabled(bool state)
{
	enabled = state;
}

bool WTOscillator::getEnabled()
{
	return enabled;
}
} /* namespace cr42y */
