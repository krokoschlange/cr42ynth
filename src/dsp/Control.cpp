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

#include "Control.h"

namespace cr42y
{

Control::Control(int id, std::function<void(float)> setter, std::function<float()> getter) :
		ID(id),
		setterFunction(setter),
		getterFunction(getter),
		min(0),
		max(1),
		controller(nullptr)
{
}

Control::~Control()
{
}

const int Control::getID()
{
	return ID;
}

void Control::setValueLFO(float lfoValue)
{
	setterFunction(min + (lfoValue + 1) / 2 * (max - min));
}

void Control::setValue(float v)
{
	if (v > max)
	{
		v = max;
	}
	else if (v < min)
	{
		v = min;
	}
	setterFunction(v);
}

void Control::setMin(float m)
{
	min = m;
}

void Control::setMax(float m)
{
	max = m;
}

float Control::getValue()
{
	return getterFunction();
}

float Control::getMin()
{
	return min;
}

float Control::getMax()
{
	return max;
}

void Control::setController(Controller* cont)
{
	controller = cont;
}

Controller* Control::getController()
{
	return controller;
}

void Control::updateValue()
{
	if (getController() != 0)
	{
		setValueLFO(controller->getValue());
	}
}

} /* namespace cr42y */
