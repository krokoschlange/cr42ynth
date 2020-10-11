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

#include "CR42YLabel.h"
#include "CR42YTheme.h"
#include "helpers.h"

#include <iostream>

namespace cr42y
{

CR42YLabel::CR42YLabel(CR42YUI* ui) :
		Glib::ObjectBase("CR42YLabel"),
		Gtk::Widget(),
		CR42YWidget(ui),
		text_(""),
		fontSize_(CR42YTheme::SMALL),
		forcedSizeRatio_(0)
{
	set_flags(Gtk::NO_WINDOW);
}

CR42YLabel::~CR42YLabel()
{

}

std::string CR42YLabel::text()
{
	return text_;
}

Cairo::RefPtr<Cairo::Surface> CR42YLabel::surfActive()
{
	return surfActive_;
}

Cairo::RefPtr<Cairo::Surface> CR42YLabel::surfInactive()
{
	return surfInactive_;
}

void CR42YLabel::setText(std::string text)
{
	text_ = text;
}

void CR42YLabel::setSurfActive(Cairo::RefPtr<Cairo::Surface> surfActive)
{
	surfActive_ = surfActive;
}

void CR42YLabel::setSurfInactive(Cairo::RefPtr<Cairo::Surface> surfInactive)
{
	surfInactive_ = surfInactive;
}

void CR42YLabel::setFontSize(int size)
{
	fontSize_ = size;
}

void CR42YLabel::setForcedSizeRatio(double forcedSizeRatio)
{
	forcedSizeRatio_ = forcedSizeRatio;
}

bool CR42YLabel::on_expose_event(GdkEventExpose* event)
{
	if (window_)
	{
		Cairo::RefPtr<Cairo::Context> cr = window_->create_cairo_context();
		cr->rectangle(event->area.x, event->area.y, event->area.width,
				event->area.height);
		cr->clip_preserve();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->fill();

		//cr42y_rounded_rectangle(cr, event->area.x, event->area.y, event->area.width, event->area.height, tm->cornerRadius());
		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(),
				tm->cornerRadius(), tm->lineThick());

		Gtk::StateType state = get_state();

		switch (state)
		{
		case Gtk::STATE_ACTIVE:
			clr = tm->color(HIGHLIGHT);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.2);
			break;
		default:
			clr = tm->color(BG_DARK);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			break;
		}
		cr->fill_preserve();
		cr->set_line_width(tm->lineThick());

		switch (state)
		{
		case Gtk::STATE_ACTIVE:
			break;
		case Gtk::STATE_INSENSITIVE:
			clr = tm->color(BG);
			break;
		default:
			clr = tm->color(FG);
			break;
		}
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->stroke();

		Cairo::RefPtr<Cairo::Surface> surf;
		if (state == Gtk::STATE_ACTIVE)
		{
			surf = surfActive_;
		}
		else
		{
			surf = surfInactive_;
		}

		switch (state)
		{
		case Gtk::STATE_ACTIVE:
			clr = tm->color(BG_DARK);
			break;
		case Gtk::STATE_INSENSITIVE:
			clr = tm->color(BG);
			break;
		default:
			clr = tm->color(FG);
			break;
		}

		if (surf)
		{
			drawSurface(cr, surf);
			cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL,
					Cairo::FONT_WEIGHT_NORMAL);
			cr->set_font_size(tm->fontSize((CR42YTheme::FONT_SIZE) fontSize_));

			Cairo::TextExtents xtents;
			cr->get_text_extents(text_, xtents);
			cr->move_to((get_width() - xtents.width) / 2, get_height());

			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

			cr->show_text(text_);
		}
		else
		{
			cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL,
					Cairo::FONT_WEIGHT_NORMAL);
			cr->set_font_size(tm->fontSize((CR42YTheme::FONT_SIZE) fontSize_));

			Cairo::TextExtents xtents;
			cr->get_text_extents(text_, xtents);
			cr->move_to((get_width() - xtents.width) / 2.,
					(get_height() + xtents.height) / 2.);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

			cr->show_text(text_);
		}
	}
}

void CR42YLabel::on_realize()
{
	Gtk::Widget::on_realize();

	if (!window_)
	{
		GdkWindowAttr attributes;
		memset(&attributes, 0, sizeof(attributes));

		Gtk::Allocation alloc = get_allocation();

		attributes.x = alloc.get_x();
		attributes.y = alloc.get_y();
		attributes.width = alloc.get_width();
		attributes.height = alloc.get_height();

		attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;

		window_ = Gdk::Window::create(get_window(), &attributes,
				GDK_WA_X | GDK_WA_Y);
		unset_flags(Gtk::NO_WINDOW);
		set_window(window_);

		window_->set_user_data(gobj());
	}
}

void CR42YLabel::on_size_allocate(Gtk::Allocation& alloc)
{
	if (forcedSizeRatio_ > 0)
	{
		double allocRatio = (double) alloc.get_width() / alloc.get_height();
		if (allocRatio > forcedSizeRatio_)
		{
			int oldWidth = alloc.get_width();
			alloc.set_width(alloc.get_height() * forcedSizeRatio_);
			alloc.set_x(alloc.get_x() + (oldWidth - alloc.get_width()) / 2);
		}
		else if (allocRatio < forcedSizeRatio_)
		{
			int oldHeight = alloc.get_height();
			alloc.set_height(alloc.get_width() / forcedSizeRatio_);
			alloc.set_y(alloc.get_y() + (oldHeight - alloc.get_height()) / 2);
		}
	}

	set_allocation(alloc);
	if (window_)
	{
		window_->move_resize(alloc.get_x(), alloc.get_y(), alloc.get_width(),
				alloc.get_height());
	}
}

void CR42YLabel::drawSurface(Cairo::RefPtr<Cairo::Context> cr,
		Cairo::RefPtr<Cairo::Surface> surf)
{
	if (surf)
	{
		int surfW;
		int surfH;
		if (surf->get_type() == Cairo::SURFACE_TYPE_IMAGE)
		{
			Cairo::RefPtr<Cairo::ImageSurface> img = Cairo::RefPtr<
					Cairo::ImageSurface>::cast_dynamic(surf);
			surfW = img->get_width();
			surfH = img->get_height();
		}
		else
		{
			surfW = get_width();
			surfH = get_height();
		}

		double scale =
				get_width() / (double) surfW > get_height() / (double) surfH ?
						get_height() / (double) surfH :
						get_width() / (double) surfW;

		cr->save();
		cr->translate((get_width() - surfW * scale) / 2,
				(get_height() - surfH * scale) / 2);
		cr->scale(scale, scale);

		cr->set_source(surf, 0, 0);
		cr->paint();
		cr->restore();

	}
}

} /* namespace cr42y */
