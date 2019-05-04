/*
 * Control.cpp
 *
 *  Created on: 01.05.2019
 *      Author: fabian
 */

#include "../../src/dsp/Control.h"

namespace cr42y
{

Control::Control(float v, float m, float ma)
{
	value = v;
	min = m;
	max = ma;
}

Control::~Control()
{
}

void Control::setLFOValue(float lfoValue)
{
	value = min + lfoValue * (max - min);
}

float Control::getValue()
{
	return value;
}

} /* namespace cr42y */
