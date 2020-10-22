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
