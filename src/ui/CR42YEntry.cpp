/*
 * CR42YEntry.cpp
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#include "CR42YEntry.h"
#include "CR42YTheme.h"
#include "helpers.h"
#include <vector>

namespace cr42y
{

CR42YEntry::CR42YEntry(CR42YUI* ui) :
		Glib::ObjectBase("CR42YEntry"),
		CR42YWidget(ui)
{
	std::string font = theme()->font() + " " + std::to_string(theme()->fontSizeMiddle());
	//modify_font(Pango::FontDescription(font));
}

CR42YEntry::~CR42YEntry()
{
}

bool CR42YEntry::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		cr->rectangle(0, 0, get_width(), get_height());
		cr->fill();

		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius());
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke();

		Glib::RefPtr<Gdk::Window> textWin = get_text_window();
		Cairo::RefPtr<Cairo::Context> textCr = textWin->create_cairo_context();
		clr = tm->color(BG_DARK);
		textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		textCr->paint();

		Glib::RefPtr<Pango::Layout> layout = get_layout();
		//cr->set_source_rgba(0, 0, 0, 1);
		clr = tm->color(FG);
		textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		int x, y;
		get_layout_offsets(x, y);
		textCr->move_to(x, y);

		layout->show_in_cairo_context(textCr);
		int start, end;
		if (get_selection_bounds(start, end))
		{
			Pango::Rectangle inkRect;
			Pango::Rectangle logicalRect;

			Glib::RefPtr<Pango::LayoutLine> line = layout->get_line(0);

			std::vector<std::pair<int, int>> ranges = line->get_x_ranges(start, end);

			layout->get_extents(inkRect, logicalRect);

			clr = tm->color(FG_DARK);
			textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

			for (int i = 0; i < ranges.size(); i++)
			{
				int rectX = x - property_scroll_offset().get_value() + ranges[i].first / Pango::SCALE;
				int rectY = y;
				int rectW = (ranges[i].second - ranges[i].first) / Pango::SCALE;
				int rectH = logicalRect.get_height() / Pango::SCALE;
				textCr->rectangle(rectX, rectY, rectW, rectH);
				textCr->fill();
			}
		}
	}
	//Gtk::Entry::on_expose_event(event);
}

} /* namespace cr42y */
