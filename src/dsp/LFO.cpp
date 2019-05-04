/*
 * LFO.cpp
 *
 *  Created on: 01.05.2019
 *      Author: fabian
 */

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
