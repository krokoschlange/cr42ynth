/*
 * CR42YScrollbar.cpp
 *
 *  Created on: 30.08.2020
 *      Author: fabian
 */

#include "CR42YVScrollbar.h"
#include "CR42YTheme.h"
#include "helpers.h"

namespace cr42y
{

CR42YVScrollbar::CR42YVScrollbar(CR42YUI* ui) :
		Glib::ObjectBase("CR42YVScrollbar"),
		Gtk::VScrollbar(),
		CR42YWidget(ui)
{
	
}

CR42YVScrollbar::~CR42YVScrollbar()
{
}

bool CR42YVScrollbar::scroll_event(GdkEventScroll* event)
{
	return on_scroll_event(event);
}

bool CR42YVScrollbar::on_expose_event(GdkEventExpose* event)
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
		x = alloc.get_x();
		Gtk::Adjustment* adj = get_adjustment();
		h = get_height() * (adj->get_page_size() / adj->get_upper() - adj->get_lower());
		y = (get_height() - h) * ((adj->get_value() - adj->get_lower()) / (adj->get_upper() - adj->get_lower() - adj->get_page_size()));
		w = get_width();

		y += alloc.get_y();

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
