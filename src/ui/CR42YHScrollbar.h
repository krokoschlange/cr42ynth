/*
 * CR42YHScrollbar.h
 *
 *  Created on: 31.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YHSCROLLBAR_H_
#define SRC_UI_CR42YHSCROLLBAR_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YHScrollbar : public Gtk::HScrollbar, public CR42YWidget
{
public:
	CR42YHScrollbar(CR42YUI* ui);
	virtual ~CR42YHScrollbar();

	bool scroll_event(GdkEventScroll* event);

protected:
	bool on_expose_event(GdkEventExpose* event);
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YHSCROLLBAR_H_ */
