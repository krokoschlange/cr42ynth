/*
 * CR42YScrollbar.h
 *
 *  Created on: 30.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YVSCROLLBAR_H_
#define SRC_UI_CR42YVSCROLLBAR_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YVScrollbar : public Gtk::VScrollbar, public CR42YWidget
{
public:
	CR42YVScrollbar(CR42YUI* ui);
	virtual ~CR42YVScrollbar();

	//kinda dirty, allows to connect scroll events from the scrollable widget to the scrollbar
	bool scroll_event(GdkEventScroll* event);

protected:
	bool on_expose_event(GdkEventExpose* event);
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YVSCROLLBAR_H_ */
