/*
 * CR42YUI.cpp
 *
 *  Created on: 04.08.2020
 *      Author: fabian
 */

#include "CR42YUI.h"

namespace cr42y
{

CR42YUI::CR42YUI() :
		CR42YRelativeContainer(nullptr)
{
	
}

CR42YUI::~CR42YUI()
{
}

void CR42YUI::setResourceRoot(std::string resourceRoot)
{
	resourceRoot_ = resourceRoot;
}

std::string CR42YUI::resourceRoot()
{
	return resourceRoot_;
}

} /* namespace cr42y */
