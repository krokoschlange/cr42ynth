/*
 * CR42YIntegerEntry.cpp
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#include "CR42YIntegerEntry.h"

namespace cr42y
{

CR42YIntegerEntry::CR42YIntegerEntry(CR42YUI* ui) :
		CR42YRelativeContainer(ui),
		increaseBtn_(nullptr),
		decreaseBtn_(nullptr)
{
	
}

CR42YIntegerEntry::~CR42YIntegerEntry()
{
	delete increaseBtn_;
	delete decreaseBtn_;
}

} /* namespace cr42y */
