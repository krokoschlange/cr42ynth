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

#include "CR42YEntry.h"
#include "CR42YTheme.h"
#include "helpers.h"
#include <vector>

namespace cr42y
{

CR42YEntry::CR42YEntry(CR42YUI* ui) :
		Glib::ObjectBase("CR42YEntry"),
		CR42YWidget(ui),
		currentFontSize_(theme()->fontSizeSmall()),
		fontSize_(CR42YTheme::SMALL),
		drawCursor_(true),
		drawSelection_(true)
{
	std::string font = theme()->font() + " "
			+ std::to_string(theme()->fontSizeSmall());
	modify_font(Pango::FontDescription(font));
}

CR42YEntry::~CR42YEntry()
{
}

void CR42YEntry::setDrawCursor(bool drawCursor)
{
	drawCursor_ = drawCursor;
}

void CR42YEntry::setDrawSelection(bool drawSelection)
{
	drawSelection_ = drawSelection;
}

void CR42YEntry::setFontSize(int fontSize)
{
	fontSize_ = fontSize;
	if (theme()->fontSize((CR42YTheme::FONT_SIZE) fontSize_)
			!= currentFontSize_)
	{
		std::string font = theme()->font() + " "
				+ std::to_string(
						theme()->fontSize((CR42YTheme::FONT_SIZE) fontSize_));
		modify_font(Pango::FontDescription(font));
		currentFontSize_ = theme()->fontSizeSmall();
	}
}

bool CR42YEntry::on_expose_event(GdkEventExpose*)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		/*std::string font = theme()->font() + " "
		 + std::to_string(theme()->fontSizeSmall());
		 modify_font(Pango::FontDescription(font));*/

		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		cr->rectangle(0, 0, get_width(), get_height());
		cr->fill();

		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(),
				tm->cornerRadius(), tm->lineThick());
		if (get_state() != Gtk::STATE_INSENSITIVE)
		{
			clr = tm->color(FG);
		}
		else
		{
			clr = tm->color(BG);
		}
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke();

		drawText();
		if (drawCursor_ && has_focus())
		{
			drawCursor();
		}
	}
	return false;
}

void CR42YEntry::on_size_allocate(Gtk::Allocation& alloc)
{
	set_size_request(alloc.get_width(), alloc.get_height());

	/* needed if statement or it will do some weird crap (modify_font probably
	 * triggers another size_allocate? idk)
	 */
	if (theme()->fontSize((CR42YTheme::FONT_SIZE) fontSize_)
			!= currentFontSize_)
	{
		std::string font = theme()->font() + " "
				+ std::to_string(
						theme()->fontSize((CR42YTheme::FONT_SIZE) fontSize_));
		modify_font(Pango::FontDescription(font));
		currentFontSize_ = theme()->fontSizeSmall();
	}
	Gtk::Entry::on_size_allocate(alloc);
}

void CR42YEntry::drawText()
{
	float* clr;
	CR42YTheme* tm = theme();
	
	Glib::RefPtr<Gdk::Window> textWin = get_text_window();
	Cairo::RefPtr<Cairo::Context> textCr = textWin->create_cairo_context();
	clr = tm->color(BG_DARK);
	textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
	textCr->paint();

	Glib::RefPtr<Pango::Layout> layout = get_layout();
	//cr->set_source_rgba(0, 0, 0, 1);
	if (get_state() != Gtk::STATE_INSENSITIVE)
	{
		clr = tm->color(FG);
	}
	else
	{
		clr = tm->color(BG);
	}
	textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
	int x, y;
	get_layout_offsets(x, y);
	int textWinX, textWinY;
	textWin->get_position(textWinX, textWinY);
	x -= textWinX;
	y -= textWinY;
	textCr->move_to(x, y);

	layout->show_in_cairo_context(textCr);
	int start, end;
	if (get_selection_bounds(start, end) && drawSelection_)
	{
		Pango::Rectangle inkRect;
		Pango::Rectangle logicalRect;

		Glib::RefPtr<Pango::LayoutLine> line = layout->get_line(0);
		
		Glib::ustring text = layout->get_text();
		
		int startIdx = g_utf8_offset_to_pointer(text.data(), start)
				- text.data();
		int endIdx = g_utf8_offset_to_pointer(text.data(), end) - text.data();

		std::vector<std::pair<int, int>> ranges = line->get_x_ranges(startIdx,
				endIdx);

		layout->get_extents(inkRect, logicalRect);

		clr = tm->color(FG_DARK);
		textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		for (size_t i = 0; i < ranges.size(); i++)
		{
			int rectX = x /*- property_scroll_offset().get_value()*/
			+ ranges[i].first / Pango::SCALE;
			int rectY = y;
			int rectW = (ranges[i].second - ranges[i].first) / Pango::SCALE;
			int rectH = logicalRect.get_height() / Pango::SCALE;
			textCr->rectangle(rectX, rectY, rectW, rectH);
		}
		textCr->clip_preserve();
		textCr->fill();
		
		textCr->move_to(x, y);
		
		clr = tm->color(BG);
		textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		layout->show_in_cairo_context(textCr);
	}
}

void CR42YEntry::drawCursor()
{
	Glib::RefPtr<Pango::Layout> layout = get_layout();
	Glib::ustring text = layout->get_text();
	
	int idx;
	
	if (true) //standard cursor (needs replacing, obv)
	{
		idx = g_utf8_offset_to_pointer(text.data(), get_position())
				- text.data();
	}
	else //some other cursor
	{
		idx = 0;
	}
	
	Pango::Rectangle strongPos = layout->get_cursor_strong_pos(idx);
	
	Glib::RefPtr<Gdk::Window> textWin = get_text_window();
	int x, y, textWinX, textWinY;
	textWin->get_position(textWinX, textWinY);
	get_layout_offsets(x, y);
	x -= textWinX;
	y -= textWinY;
	
	int cursorX = strongPos.get_x() / Pango::SCALE + x; //- property_scroll_offset();

	Cairo::RefPtr<Cairo::Context> textCr = textWin->create_cairo_context();
	
	CR42YTheme* tm = theme();
	float* clr = tm->color(FG);
	textCr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
	
	int textAreaWidth, textAreaHeight;
	textWin->get_size(textAreaWidth, textAreaHeight);
	
	textCr->rectangle(cursorX, 2, 2, textAreaHeight - 4);
	textCr->fill();
}

} /* namespace cr42y */
