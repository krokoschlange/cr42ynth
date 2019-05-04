/*
 * WTOscillator.cpp
 *
 *  Created on: 26.04.2019
 *      Author: fabian
 */

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
