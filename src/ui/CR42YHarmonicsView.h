/*
 * CR42YHarmonicsView.h
 *
 *  Created on: 03.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YHARMONICSVIEW_H_
#define SRC_UI_CR42YHARMONICSVIEW_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class WavetableEditController;

class CR42YHarmonicsView : public Gtk::Widget, public CR42YWidget
{
public:
	CR42YHarmonicsView(CR42YUI* ui);
	virtual ~CR42YHarmonicsView();

	void setController(WavetableEditController* controller);

protected:
	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_realize();
	virtual void on_size_allocate(Gtk::Allocation& alloc);

private:
	Glib::RefPtr<Gdk::Window> window_;

	WavetableEditController* controller_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YHARMONICSVIEW_H_ */
