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
/*
 * LineEntry.cpp
 *
 *  Created on: 07.06.2020
 *      Author: fabian
 */

#include "LineEntry.h"

#include "avtk.hxx"

namespace cr42y
{

LineEntry::LineEntry(Avtk::UI* ui, int x, int y, int w, int h, std::string lbl) :
		Avtk::Widget(ui, x, y, w, h, lbl),
		text(""),
		cursorPos(0),
		scrollPos(0),
		focus(false),
		allowedChars(" !\"#$%&'()*+´-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"),
		lastClickedPos(-1)

{
}

LineEntry::~LineEntry()
{
}

void LineEntry::setText(std::string txt)
{
	text = txt;
	if (cursorPos > text.length())
	{
		cursorPos = text.length();
	}
}

std::string LineEntry::getText()
{
	return text;
}

void LineEntry::setCursorPos(int pos)
{
	pos = pos > text.length() ? text.length() : pos;
	pos = pos < 0 ? 0 : pos;
	cursorPos = pos;

	/*int cursorPixelPos = toPixelPos(cursorPos);
	 if (cursorPixelPos < scrollPos)
	 {
	 scrollPos = cursorPixelPos;
	 }
	 else if (cursorPixelPos > scrollPos + w())
	 {
	 scrollPos = cursorPixelPos - w();
	 }*/
}

int LineEntry::getCursorPos()
{
	return cursorPos;
}

void LineEntry::setScrollPos(float pos)
{
	scrollPos = pos;
}

float LineEntry::getScrollPos()
{
	return scrollPos;
}

void LineEntry::setAllowedChars(std::string chars)
{
	allowedChars = chars;
}

std::string LineEntry::getAllowedChars()
{
	return allowedChars;
}

int LineEntry::toTextPos(cairo_t* cr, int x)
{
	cairo_text_extents_t extents;
	cairo_text_extents(cr, "", &extents);
	double w = 0;
	int i;
	for (i = 0; w < x && i <= text.length(); i++)
	{
		cairo_text_extents(cr, text.substr(i, 1).c_str(), &extents);
		w += extents.x_advance;
	}

	if (w - extents.x_advance / 2 > x)
	{
		i--;
	}

	return i;
}

int LineEntry::toPixelPos(cairo_t* cr, int pos)
{
	cairo_text_extents_t extents;
	cairo_text_extents(cr, text.substr(0, pos).c_str(), &extents);
	return (float) extents.width;
}

void LineEntry::draw(cairo_t* cr)
{
	cairo_save(cr);

	roundedBox(cr, x(), y(), w(), h(), theme_->cornerRadius_);

	cairo_rectangle(cr, x(), y(), w(), h());
	cairo_clip(cr);
	cairo_set_font_size(cr, 15.0);

	if (lastClickedPos > 0)
	{
		setCursorPos(toTextPos(cr, lastClickedPos));
		lastClickedPos = -1;
	}

	int cursorPixelPos = toPixelPos(cr, cursorPos);
	if (cursorPixelPos < scrollPos)
	{
		scrollPos = cursorPixelPos;
	}
	else if (cursorPixelPos > scrollPos + w())
	{
		scrollPos = cursorPixelPos - w();
	}

	const char* str = text.c_str();

	cairo_font_extents_t fextents;
	cairo_font_extents(cr, &fextents);

	cairo_text_extents_t extents;
	cairo_text_extents(cr, str, &extents);
	cairo_move_to(cr, x_ - scrollPos, (y_ + h_ / 2) + fextents.height / 2 - 2);
	theme_->color(cr, Avtk::FG);
	cairo_show_text(cr, str);

	if (focus)
	{
		cairo_rectangle(cr, x(), y(), w(), h());
		cairo_clip(cr);
		//cairo_text_extents(cr, text.substr(0, cursorPos).c_str(), &extents);
		float cursorPixelPos = toPixelPos(cr, cursorPos);
		cairo_move_to(cr, x_ - scrollPos + cursorPixelPos, (y_ + h_ / 2) + fextents.height / 2 - 2);
		cairo_line_to(cr, x_ - scrollPos + cursorPixelPos, (y_ + h_ / 2) - fextents.height / 2 - 2);
	}

	cairo_restore(cr);
}

int LineEntry::handle(const PuglEvent* event)
{
	if (!visible())
	{
		return 0;
	}
	int ret;
	if (event->type == PUGL_BUTTON_PRESS && event->button.button == 1)
	{
		if (touches(event->button.x, event->button.y))
		{
			lastClickedPos = event->button.x - x(); // - x();
			focus = true;
			ret = 1;
		}
		else
		{
			focus = false;
		}
	}
	else if (event->type == PUGL_KEY_PRESS && focus)
	{
		uint32_t key = event->key.character;
		if (allowedChars.find(key) != std::string::npos)
		{
			text.insert(text.begin() + cursorPos, key);
			setCursorPos(getCursorPos() + 1);
			ret = 1;
		}
		else if (key == PUGL_CHAR_BACKSPACE && cursorPos > 0)
		{
			std::cout << "BS on " << cursorPos << "\n";
			text.erase(text.begin() + cursorPos - 1);
			setCursorPos(getCursorPos() - 1);
			ret = 1;
		}
		else if (key == PUGL_CHAR_DELETE && cursorPos < text.length())
		{
			text.erase(text.begin() + cursorPos);
			ret = 1;
		}
		else if (event->key.character == 0)
		{
			if (event->key.special == PUGL_KEY_LEFT)
			{
				setCursorPos(getCursorPos() - 1);
				ret = 1;
			}
			if (event->key.special == PUGL_KEY_RIGHT)
			{
				setCursorPos(getCursorPos() + 1);
				ret = 1;
			}
			if (event->key.special == PUGL_KEY_HOME)
			{
				setCursorPos(0);
				ret = 1;
			}
			if (event->key.special == PUGL_KEY_END)
			{
				setCursorPos(text.length());
				ret = 1;
			}
		}
	}
	if (ret != 0)
	{
		ui->redraw(this);
	}
	return 0;
}

} /* namespace cr42y */
