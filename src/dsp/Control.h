/*
 * Control.h
 *
 *  Created on: 01.05.2019
 *      Author: fabian
 */

#ifndef DSP_CONTROL_H_
#define DSP_CONTROL_H_

namespace cr42y
{

class Control
{
public:
	Control(float v = 0, float m = 0, float ma = 1);
	virtual ~Control();

	void setValue(float v)
	{
		value = v;
	}

	void setLFOValue(float lfoValue);

	float getValue();

	void setMin(float m)
	{
		min = m;
	}

	void setMax(float m)
	{
		max = m;
	}

private:
	float value;

	float min;
	float max;
};

} /* namespace cr42y */

#endif /* DSP_CONTROL_H_ */
