#ifndef SRC_UI_CR42YCONTROLDIAL_H_
#define SRC_UI_CR42YCONTROLDIAL_H_

#include "CR42YDial.h"

#include "ControlConnector.h"

namespace cr42y
{

class CR42YControlDial : public CR42YDial
{
public:
	CR42YControlDial(CR42YUI* ui);
	virtual ~CR42YControlDial();

	void connectControl(Control* control);

private:
	ControlConnector connector_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YCONTROLDIAL_H_ */
