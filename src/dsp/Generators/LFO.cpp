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

namespace cr42y
{

LFO::LFO(float rate, PortCommunicator* comm) :
		samplerate(rate), waveform(nullptr), frequency(1, comm,
				DefinitionHandler::getInstance()->msg_key, 1, 0, 0), //TODO
		smooth(2, comm)
{
}

LFO::~LFO()
{
}

float LFO::getSample(float* wavePos)
{
	if (!waveform)
	{
		*wavePos += frequency.getValue() / samplerate;
		return 0;
	}
	int waveSample = wavePos * waveform->size();
	float out;
	if (smooth.getValue())
	{
		float smpl1 = (*waveform)[waveSample];

		float waveSample2 = waveSample + 1;
		if (waveSample2 > waveform->size())
		{
			waveSample2 -= waveform->size();
		}
		float smpl2 = (*waveform)[waveSample2];

		out = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
	}
	else
	{
		out = (*waveform)[waveSample];
	}

	*wavePos += frequency.getValue() / samplerate;
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

float LFO::getFrequency()
{
	return frequency.getValue();
}

} /* namespace cr42y */
