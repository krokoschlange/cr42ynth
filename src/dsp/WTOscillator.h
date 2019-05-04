/*
 * WTOscillator.h
 *
 *  Created on: 26.04.2019
 *      Author: fabian
 */

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
