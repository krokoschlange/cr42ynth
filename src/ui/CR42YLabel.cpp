/*
 * CR42YLabel.cpp
 *
 *  Created on: 15.07.2020
 *      Author: fabian
 */

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
		text_("")
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

bool CR42YLabel::on_expose_event(GdkEventExpose* event)
{
	if (window_)
	{
		Cairo::RefPtr<Cairo::Context> cr = window_->create_cairo_context();
		cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
		cr->clip_preserve();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->fill();


		//cr42y_rounded_rectangle(cr, event->area.x, event->area.y, event->area.width, event->area.height, tm->cornerRadius());
		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius(), tm->lineThick());

		if (get_state() == Gtk::STATE_ACTIVE)
		{
			clr = tm->color(HIGHLIGHT);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.2);
			cr->fill_preserve();
			cr->set_line_width(tm->lineThick());
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->stroke();

			if (surfActive_)
			{
				drawSurface(cr, surfActive_);

				cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
				cr->set_font_size(tm->fontSizeSmall());

				Cairo::TextExtents xtents;
				cr->get_text_extents(text_, xtents);
				cr->move_to((get_width() - xtents.width) / 2, get_height());
				clr = tm->color(BG_DARK);
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

				cr->show_text(text_);
			}
			else
			{
				cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
				cr->set_font_size(tm->fontSizeMiddle());

				Cairo::TextExtents xtents;
				cr->get_text_extents(text_, xtents);
				cr->move_to((get_width() - xtents.width) / 2., (get_height() + xtents.height) / 2.);
				clr = tm->color(BG_DARK);
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

				cr->show_text(text_);
			}
		}
		else
		{
			clr = tm->color(BG_DARK);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->fill_preserve();
			clr = tm->color(FG);
			cr->set_line_width(tm->lineThick());
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->stroke();

			if (surfInactive_)
			{
				drawSurface(cr, surfInactive_);
				cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
				cr->set_font_size(tm->fontSizeSmall());

				Cairo::TextExtents xtents;
				cr->get_text_extents(text_, xtents);
				cr->move_to((get_width() - xtents.width) / 2, get_height());
				clr = tm->color(FG);
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

				cr->show_text(text_);
			}
			else
			{
				cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
				cr->set_font_size(tm->fontSizeMiddle());

				Cairo::TextExtents xtents;
				cr->get_text_extents(text_, xtents);
				cr->move_to((get_width() - xtents.width) / 2., (get_height() + xtents.height) / 2.);
				clr = tm->color(FG);
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

				cr->show_text(text_);

			}
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

		window_ = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);
		unset_flags(Gtk::NO_WINDOW);
		set_window(window_);

		window_->set_user_data(gobj());
	}
}

void CR42YLabel::on_size_allocate(Gtk::Allocation& alloc)
{
	set_allocation(alloc);
	if (window_)
	{
		window_->move_resize(alloc.get_x(), alloc.get_y(), alloc.get_width(), alloc.get_height());
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
						get_height() / (double) surfH : get_width() / (double) surfW;

		cr->save();
		cr->translate((get_width() - surfW * scale) / 2, (get_height() - surfH * scale) / 2);
		cr->scale(scale, scale);



		cr->set_source(surf, 0, 0);
		cr->paint();
		cr->restore();

	}
}

} /* namespace cr42y */