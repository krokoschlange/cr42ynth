/*******************************************************************************
 * Copyright (c) 2019 krokoschlange and contributors.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "../../src/dsp/LFO.h"

namespace cr42y
{

LFO::LFO(float rate)
{
	samplerate = rate;
	smooth = false;
	wavePos = 0;
	frequency = 1;
}

LFO::~LFO()
{
	// TODO Auto-generated destructor stub
}

void LFO::run(int samples, float* output)
{
	float sampleLenght = frequency / samplerate;
	for (int i = 0; i < samples; i++)
	{
		if (smooth)
		{
			float smpl1 = waveform[(int) wavePos];
			float smpl2 = waveform[(int) wavePos + 1];

			float waveSample = wavePos * waveform.size();
			output[i] = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
		}
		else
		{
			output[i] = waveform[(int) wavePos];
		}

		wavePos += sampleLenght;
		if (wavePos > 1)
		{
			wavePos = wavePos - (int) wavePos;
		}
	}

	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->setLFOValue(output[samples - 1]);
	}
}

} /* namespace cr42y */
