/*
 * DoubleControl.cpp
 *
 *  Created on: 28.05.2019
 *      Author: fabian
 */

#include <lv2/atom/util.h>

#include "DoubleControl.h"
#include "DefinitionHandler.h"

namespace cr42y
{

DoubleControl::DoubleControl(int msgType, PortCommunicator* comm,
		LV2_URID dKey, double val, double mi, double ma) :
		MessageReceiver(msgType, comm), value(val), min(mi), max(ma), dataKey(dKey)
{
}

DoubleControl::~DoubleControl()
{
	disconnect();
}

void DoubleControl::receive(LV2_Atom_Object* data)
{
	LV2_Atom_Double* val;
	lv2_atom_object_get_typed(data, dataKey, &val,
			DefinitionHandler::getInstance()->atom_double);
	setValue(val->body);
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
