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

#ifndef SRC_OLD_DSP_WTOSCILLATOR_H_
#define SRC_OLD_DSP_WTOSCILLATOR_H_

#include "lv2/log/log.h"
#include "lv2/log/logger.h"

#include <vector>

namespace cr42y
{

class WTOscillator
{
public:
	WTOscillator(int rate);
	virtual ~WTOscillator() {}

	void run(int samples, float* output);

	void setWavetable(int length, int height, bool ipolate, std::vector<std::vector<float>>);

	void setSmooth(bool sm)
	{
		smooth = sm;
	}

	void setFrequency(float f)
	{
		frequency = f;
	}

	float getFrequency()
	{
		return frequency;
	}

	void updateRate(int rate)
	{
		samplerate = rate;
	}

	void setWTPos(float pos)
	{
		wtPos = pos;
	}

	float getWTPos()
	{
		return wtPos;
	}

private:
	float frequency;
	int samplerate;
	bool smooth;
	float wavePos;

	int wtLength;
	int wtHeight;
	std::vector<std::vector<float>> wavetable;
	float wtPos;
	bool interpolate;
};

} /* namespace cr42y */

#endif /* SRC_OLD_DSP_WTOSCILLATOR_H_ */
