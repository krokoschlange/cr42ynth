/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 * 3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior
 *     written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

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
