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
		ControlListener(),
		control_(nullptr),
		ignoreCallback_(false)
{
	widgetSetValue_ = [](float)
	{};
	widgetSetMin_ = [](float)
	{};
	widgetSetMax_ = [](float)
	{};
	widgetSetGenerator_ = [](std::string)
	{};
}

ControlConnector::~ControlConnector()
{
}

void ControlConnector::setControlValue(double value)
{
	if (control_)
	{
		control_->setValue(value, true);
		ignoreCallback_ = true;
	}
}

void ControlConnector::setControlMin(double min)
{
	if (control_)
	{
		control_->setMin(min, true);
		ignoreCallback_ = true;
	}
}

void ControlConnector::setControlMax(double max)
{
	if (control_)
	{
		control_->setValue(max, true);
		ignoreCallback_ = true;
	}
}

void ControlConnector::setControlGenerator(std::string generator)
{
	if (control_)
	{
		control_->setGenerator(generator, true);
		ignoreCallback_ = true;
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

void ControlConnector::valueCallback(float val, Control*)
{
	if (!ignoreCallback_)
	{
		widgetSetValue_(val);
		ignoreCallback_ = false;
	}
}

void ControlConnector::minCallback(float min, Control*)
{
	if (!ignoreCallback_)
	{
		widgetSetMin_(min);
		ignoreCallback_ = false;
	}
}

void ControlConnector::maxCallback(float max, Control*)
{
	if (!ignoreCallback_)
	{
		widgetSetMax_(max);
		ignoreCallback_ = false;
	}
}

void ControlConnector::genCallback(std::string gen, Control*)
{
	if (!ignoreCallback_)
	{
		widgetSetGenerator_(gen);
		ignoreCallback_ = false;
	}
}

void ControlConnector::connect(Control& ctrl)
{
	for (size_t i = 0; i < connections_.size(); i++)
	{
		ControlListener::disconnect(*connections_[i]);
	}
	control_ = &ctrl;
}

void ControlConnector::disconnect(Control& ctrl)
{
	ControlListener::disconnect(ctrl);
	if (&ctrl == control_)
	{
		control_ = nullptr;
	}
}

Control* ControlConnector::getControl()
{
	return control_;
}

} /* namespace cr42y */
