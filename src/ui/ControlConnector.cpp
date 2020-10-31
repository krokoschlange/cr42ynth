/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
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
#include "ControlConnector.h"

namespace cr42y
{

ControlConnector::ControlConnector() :
		ControlListener()
{
	widgetSetValue_ = [](float value)
	{};
	widgetSetMin_ = [](float min)
	{};
	widgetSetMax_ = [](float max)
	{};
	widgetSetGenerator_ = [](std::string generator)
	{};
}

ControlConnector::~ControlConnector()
{
}

void ControlConnector::setControlValue(double value)
{
	if (getControl())
	{
		getControl()->setValue(value, true);
	}
}

void ControlConnector::setControlMin(double min)
{
	if (getControl())
	{
		getControl()->setMin(min, true);
	}
}

void ControlConnector::setControlMax(double max)
{
	if (getControl())
	{
		getControl()->setValue(max, true);
	}
}

void ControlConnector::setControlGenerator(std::string generator)
{
	if (getControl())
	{
		getControl()->setGenerator(generator, true);
	}
}

void ControlConnector::setWidgetValueSetter(std::function<void(double)> widgetSetValue)
{
	widgetSetValue_ = widgetSetValue;
}

void ControlConnector::setWidgetMinSetter(std::function<void(double)> widgetSetMin)
{
	widgetSetMin_ = widgetSetMin;
}

void ControlConnector::setWidgetMaxSetter(std::function<void(double)> widgetSetMax)
{
	widgetSetMax_ = widgetSetMax;
}

void ControlConnector::setWidgetGeneratorSetter(
		std::function<void(std::string)> widgetSetGenerator)
{
	widgetSetGenerator_ = widgetSetGenerator;
}

void ControlConnector::valueCallback(float val)
{
	widgetSetValue_(val);
}

void ControlConnector::minCallback(float min)
{
	widgetSetMin_(min);
}

void ControlConnector::maxCallback(float max)
{
	widgetSetMax_(max);
}

void ControlConnector::genCallback(std::string gen)
{
	widgetSetGenerator_(gen);
}

} /* namespace cr42y */
