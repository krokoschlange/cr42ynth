#include "CR42YControlDial.h"

namespace cr42y
{

CR42YControlDial::CR42YControlDial(CR42YUI* ui) :
		Glib::ObjectBase("CR42YControlDial"),
		CR42YDial(ui),
		connector_()
{
	using std::placeholders::_1;
	connector_.setWidgetValueSetter((std::function<void(double)>)std::bind(&CR42YControlDial::setValue, this, _1, false));
	signalChanged().connect(sigc::mem_fun(&connector_, &ControlConnector::setControlValue));
}

CR42YControlDial::~CR42YControlDial()
{
}

void CR42YControlDial::connectControl(Control* control)
{
	connector_.listenTo(control);
	setValue(connector_.getControl()->getValue(), false);
}

} /* namespace cr42y */
