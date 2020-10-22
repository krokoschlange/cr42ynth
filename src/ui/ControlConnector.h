#ifndef SRC_UI_CONTROLCONNECTOR_H_
#define SRC_UI_CONTROLCONNECTOR_H_

#include "ControlListener.h"

#include <functional>

namespace cr42y
{

class ControlConnector : public ControlListener
{
public:
	ControlConnector();
	virtual ~ControlConnector();

	void setControlValue(double value);
	void setControlMin(double min);
	void setControlMax(double max);
	void setControlGenerator(std::string generator);

	void setWidgetValueSetter(std::function<void(double)> widgetSetValue);
	void setWidgetMinSetter(std::function<void(double)> widgetSetMin);
	void setWidgetMaxSetter(std::function<void(double)> widgetSetMax);
	void setWidgetGeneratorSetter(std::function<void(std::string)> widgetSetGenerator);

	void valueCallback(float val);
	void minCallback(float min);
	void maxCallback(float max);
	void genCallback(std::string gen);

private:
	std::function<void(double)> widgetSetValue_;
	std::function<void(double)> widgetSetMin_;
	std::function<void(double)> widgetSetMax_;
	std::function<void(std::string)> widgetSetGenerator_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CONTROLCONNECTOR_H_ */
