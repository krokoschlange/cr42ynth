/*
 * CR42YHScrollbar.cpp
 *
 *  Created on: 31.08.2020
 *      Author: fabian
 */

#include "CR42YHScrollbar.h"
#include "CR42YTheme.h"
#include "helpers.h"

namespace cr42y
{

CR42YHScrollbar::CR42YHScrollbar(CR42YUI* ui) :
		Glib::ObjectBase("CR42YHScrollbar"),
		Gtk::HScrollbar(),
		CR42YWidget(ui)
{
	
}

CR42YHScrollbar::~CR42YHScrollbar()
{
}

bool CR42YHScrollbar::scroll_event(GdkEventScroll* event)
{
	return on_scroll_event(event);
}

bool CR42YHScrollbar::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG);

		Gtk::Allocation alloc = get_allocation();

		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->rectangle(alloc.get_x(), alloc.get_y(), get_width(), get_height());
		cr->fill();

		int x, y, w, h;
		y = alloc.get_y();
		Gtk::Adjustment* adj = get_adjustment();
		w = get_width() * (adj->get_page_size() / adj->get_upper() - adj->get_lower());
		x = (get_width() - w) * ((adj->get_value() - adj->get_lower()) / (adj->get_upper() - adj->get_lower() - adj->get_page_size()));
		h = get_height();

		x += alloc.get_x();

		cr42y_rounded_rectangle(cr, x, y, w, h, tm->cornerRadius(), tm->lineThick());
		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.2);
		cr->fill_preserve();
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke();
	}
}

} /* namespace cr42y */
