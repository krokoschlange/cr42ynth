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

#include "WTOscillator.h"

#include <cmath>
#include <iostream>

namespace cr42y
{

WTOscillator::WTOscillator(int rate)
{
	samplerate = rate;
	frequency = 440;
	smooth = false;
	wavePos = 0;

	wtLength = 2048;
	wtHeight = 0;
	wtPos = 0;
	interpolate = false;

}

void WTOscillator::setWavetable(int length, int height, bool ipolate,
		std::vector<std::vector<float>> table)
{
	wtLength = length;
	wtHeight = height;

	wavetable = table;
}

void WTOscillator::run(int samples, float* output)
{
	float deltaPos = frequency / samplerate;

	for (int i = 0; i < samples; i++)
	{
		float wtSample = wavePos * wtLength;

		float out;

		if (smooth)
		{
			if (interpolate && !(wtPos - wtHeight < 0.0001)) //if we are at the end of the WT, we dont need to interpolate
			{
				//linear interpolation between waveforms and then samples
				float smplA1 = wavetable[(int) wtPos][(int) wtSample];
				float smplA2 = wavetable[(int) wtPos][((int) wtSample) + 1];
				float smplB1 = wavetable[((int) wtPos) + 1][(int) wtSample];
				float smplB2 =
						wavetable[((int) wtPos) + 1][((int) wtSample) + 1];

				float interpolated1 = smplA1
						+ (wtPos - (int) wtPos) * (smplB1 - smplA1);
				float interpolated2 = smplA2
						+ (wtPos - (int) wtPos) * (smplB2 - smplA2);

				out = interpolated1
						+ (wtSample - (int) wtSample)
								* (interpolated2 - interpolated1);
			}
			else
			{
				//linear interpolation between samples
				float smpl1 = wavetable[(int) wtPos][(int) wtSample];
				float smpl2 = wavetable[(int) wtPos][((int) wtSample) + 1];

				out = smpl1 + (wtSample - (int) wtSample) * (smpl2 - smpl1);
			}

		}
		else
		{
			if (interpolate && !(wtPos - wtHeight < 0.0001))
			{
				//linear interpolation between waveforms
				float smplA = wavetable[(int) wtPos][(int) wtSample];
				float smplB = wavetable[((int) wtPos) + 1][(int) wtSample];

				out = smplA + (wtSample - (int) wtSample) * (smplB - smplA);
			}
			else
			{
				//lv2_log_note(logger,
				//		(std::to_string(wtHeight * wtPos) + "|").c_str());
				out = wavetable[(int) wtPos][(int) wtSample];
			}

		}

		output[i] = out;

		wavePos += deltaPos;

		if (wavePos > 1)
		{
			wavePos = wavePos - (int) wavePos;
		}
	}

}

} /* namespace cr42y */
