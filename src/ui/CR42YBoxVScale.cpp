/*
 * CR42YBoxVScale.cpp
 *
 *  Created on: 28.07.2020
 *      Author: fabian
 */

#include "CR42YBoxVScale.h"
#include "CR42YTheme.h"

#include <iostream>

namespace cr42y
{

CR42YBoxVScale::CR42YBoxVScale(CR42YUI* ui) :
		Glib::ObjectBase("CR42YBoxVScale"),
		Gtk::Widget(),
		CR42YWidget(ui),
		doubleSided_(false)
{
	set_flags(Gtk::NO_WINDOW);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YBoxVScale::on_button_press));
	signal_button_release_event().connect(sigc::mem_fun(this, &CR42YBoxVScale::on_button_release));
	signal_motion_notify_event().connect(sigc::mem_fun(this, &CR42YBoxVScale::on_motion_notify));
}

CR42YBoxVScale::~CR42YBoxVScale()
{
}

bool CR42YBoxVScale::doubleSided()
{
	return doubleSided_;
}

void CR42YBoxVScale::setDoubleSided(bool ds)
{
	doubleSided_ = ds;
}

double CR42YBoxVScale::value()
{
	return value_;
}

void CR42YBoxVScale::setValue(double value)
{
	value_ = fmax(0, fmin(value, 1));
}

bool CR42YBoxVScale::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);

		cr->rectangle(0, 0, get_width(), get_height());
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->fill();

		if (doubleSided())
		{
			cr->move_to(0, get_height() / 2.0);
			cr->line_to(get_width(), get_height() / 2.0);
			float* clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineMiddle());
			cr->stroke();

			cr->move_to(get_width() / 8.0, get_height() / 2.0);
			cr->line_to(get_width() / 8.0, get_height() * (1 - value()));
			cr->line_to(get_width() - get_width() / 8.0, get_height() * (1 - value()));
			cr->line_to(get_width() - get_width() / 8.0, get_height() / 2.0);
			cr->set_line_width(tm->lineThick());
			clr = tm->color(HIGHLIGHT);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->stroke_preserve();

			cr->close_path();
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.2);
			cr->fill();
		}
		else
		{
			cr->move_to(0, get_height());
			cr->line_to(get_width(), get_height());
			float* clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineMiddle());
			cr->stroke();

			cr->move_to(get_width() / 8.0, get_height());
			cr->line_to(get_width() / 8.0, get_height() * (1 - value()));
			cr->line_to(get_width() - get_width() / 8.0, get_height() * (1 - value()));
			cr->line_to(get_width() - get_width() / 8.0, get_height());
			cr->set_line_width(tm->lineThick());
			clr = tm->color(HIGHLIGHT);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->stroke_preserve();

			cr->close_path();
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.2);
			cr->fill();
		}
	}
}

void CR42YBoxVScale::on_realize()
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

bool CR42YBoxVScale::on_button_press(GdkEventButton* event)
{
	if (event->button == 1)
	{
		setValue(1 - event->y / get_height());
		gtk_grab_add(gobj());
		queue_draw();
		return true;
	}
}

bool CR42YBoxVScale::on_button_release(GdkEventButton* event)
{
	if (event->button == 1)
	{
		gtk_grab_remove(gobj());
		return true;
	}
}

bool CR42YBoxVScale::on_motion_notify(GdkEventMotion* event)
{
	setValue(1 - event->y / get_height());
	queue_draw();
	return true;
}

} /* namespace cr42y */
