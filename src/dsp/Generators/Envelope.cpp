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

#include "Envelope.h"

namespace cr42y
{

Envelope::Envelope(float rate, PortCommunicator* comm) :
				samplerate(rate),
				length(1, comm, 0, 0, 0),
				smooth(1, comm),
				sustainPoint(1, comm, 0, 0, 1) //TODO
{
	
}

Envelope::~Envelope()
{
}

float Envelope::getSample(float* pos)
{
	float deltaPos = 1 / (length.getValue() * samplerate);
	float out = 0;

	if (waveform && *pos < 1)
	{
		float waveSample = *pos * waveform->size();
		if (getSmooth())
		{
			float smpl1 = (*waveform)[(int) waveSample];
			float smpl2;
			if (waveSample + 1 < waveform->size())
			{
				smpl2 = (*waveform)[(int) waveSample + 1];
			}
			else
			{
				smpl2 = 0;
			}

			out = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
		}
		else
		{
			out = (*waveform)[(int) waveSample];
		}
	}
	
	*pos += deltaPos;
	return out;
}

void Envelope::setEnvelope(std::vector<float>* env, float len)
{
	if (waveform)
	{
		delete waveform;
	}
	waveform = env;
	length.setValue(len);
}

void Envelope::setLength(float len)
{
	length.setValue(len);
}

void Envelope::setSmooth(bool state)
{
	smooth.setValue(state);
}

void Envelope::setSustainPoint(float pos)
{
	sustainPoint.setValue(pos);
}

float Envelope::getLength()
{
	return length.getValue();
}

bool Envelope::getSmooth()
{
	return smooth.getValue();
}

float Envelope::getSustainPoint()
{
	return sustainPoint.getValue();
}

} /* namespace cr42y */
