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

#include "DoubleControl.h"

namespace cr42y
{

DoubleControl::DoubleControl(double val, double mi, double ma) :
		value(val),
		min(mi),
		max(ma)
{
	receiver = nullptr;
}

DoubleControl::~DoubleControl()
{
	if (receiver)
	{
		delete receiver;
	}
}

void DoubleControl::connectPort(PortCommunicator* comm, int msgType, LV2_URID dataKey)
{

}

void DoubleControl::disconnectPort()
{

}

void DoubleControl::setValue(double val)
{
	if (val > max)
	{
		val = max;
	}
	else if (val < min)
	{
		val = min;
	}
	value = val;
}

double DoubleControl::getValue()
{
	return value;
}

void DoubleControl::setMax(double m)
{
	max = m;
}

void DoubleControl::setMin(double m)
{
	min = m;
}

double DoubleControl::getMax()
{
	return max;
}

double DoubleControl::getMin()
{
	return min;
}
} /* namespace cr42y */