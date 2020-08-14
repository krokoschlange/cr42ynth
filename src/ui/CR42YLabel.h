/*
 * CR42YLabel.h
 *
 *  Created on: 15.07.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YLABEL_H_
#define SRC_UI_CR42YLABEL_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YLabel : public Gtk::Widget, public CR42YWidget
{
public:
	CR42YLabel(CR42YUI* ui);
	virtual ~CR42YLabel();

	std::string text();
	Cairo::RefPtr<Cairo::Surface> surfActive();
	Cairo::RefPtr<Cairo::Surface> surfInactive();

	void setText(std::string text);
	void setSurfActive(Cairo::RefPtr<Cairo::Surface> surfActive);
	void setSurfInactive(Cairo::RefPtr<Cairo::Surface> surfInactive);

protected:

	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_realize();
	virtual void on_size_allocate(Gtk::Allocation& alloc);

private:

	void drawSurface(Cairo::RefPtr<Cairo::Context> cr,
			Cairo::RefPtr<Cairo::Surface> surf);

	std::string text_;

	Cairo::RefPtr<Cairo::Surface> surfActive_;
	Cairo::RefPtr<Cairo::Surface> surfInactive_;

	Glib::RefPtr<Gdk::Window> window_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YLABEL_H_ */
