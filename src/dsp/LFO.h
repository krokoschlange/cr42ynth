/*
 * LFO.h
 *
 *  Created on: 01.05.2019
 *      Author: fabian
 */

#ifndef DSP_LFO_H_
#define DSP_LFO_H_

#include <vector>
#include <list>
#include "../../src/dsp/Control.h"

namespace cr42y
{

class LFO
{
public:
	LFO(float rate);
	virtual ~LFO();

	void run(int samples, float* output);

	void setWaveform(std::vector<float> wave)
	{
		waveform = wave;
	}

	void setFrequency(float f)
	{
		frequency = f;
	}

	void setSmooth(bool s)
	{
		smooth = s;
	}

	void updateSamplerate(float rate)
	{
		samplerate = rate;
	}

	void addControl(Control* ctrl)
	{
		controls.push_back(ctrl);
	}

	void removeControl(Control* ctrl)
	{
		int index = 0;
		for (; index < controls.size() || controls[index] == ctrl; index++) {}

		controls.erase(controls.begin() + index);
	}

private:
	std::vector<float> waveform;

	float samplerate;
	float frequency;
	float wavePos;
	bool smooth;

	std::vector<Control*> controls;
};

} /* namespace cr42y */

#endif /* DSP_LFO_H_ */
