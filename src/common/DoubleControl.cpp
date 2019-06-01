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

#include <lv2/atom/util.h>

#include "DoubleControl.h"
#include "DefinitionHandler.h"

namespace cr42y
{

DoubleControl::DoubleControl(int msgType, PortCommunicator* comm, double val,
		double mi, double ma, LV2_URID dKey) :
				MessageReceiver(msgType, comm),
				value(val),
				min(mi),
				max(ma),
				dataKey(dKey)
{
}

DoubleControl::~DoubleControl()
{
}

void DoubleControl::receive(LV2_Atom_Object* data)
{
	LV2_Atom_Double* val;
	lv2_atom_object_get_typed(data, dataKey, &val,
			DefinitionHandler::getInstance()->atom_double);
	if (val)
	{
		setValue(val->body);
	}
}

double DoubleControl::getValue()
{
	return value;
}

double DoubleControl::getMin()
{
	return min;
}

double DoubleControl::getMax()
{
	return max;
}

void DoubleControl::setValue(double val)
{
	if (!(min == 0 && max == 0))
	{
		if (val < min)
		{
			val = min;
		}
		else if (val > max)
		{
			val = max;
		}
	}
	value = val;
}

void DoubleControl::setMin(double m)
{
	min = m;
}

void DoubleControl::setMax(double m)
{
	min = m;
}

} /* namespace cr42y */
