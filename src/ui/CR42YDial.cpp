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

#include "CR42YDial.h"

#include "CR42YTheme.h"

namespace cr42y
{

CR42YDial::CR42YDial(CR42YUI* ui) :
		Glib::ObjectBase("CR42YDial"),
		Gtk::Widget(),
		CR42YWidget(ui),
		value_(0),
		preClickValue_(0),
		prePreClickValue_(0),
		mouseY_(0),
		oldValue_(0),
		defaultValue_(0),
		text_("")
{
	set_flags(Gtk::NO_WINDOW);
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YDial::on_button_press));
	signal_button_release_event().connect(sigc::mem_fun(this, &CR42YDial::on_button_release));
	signal_motion_notify_event().connect(sigc::mem_fun(this, &CR42YDial::on_motion_notify));
}

CR42YDial::~CR42YDial()
{
}

double CR42YDial::value()
{
	return value_;
}

void CR42YDial::setValue(double value, bool trigger)
{
	double newVal = fmax(0, fmin(value, 1));
	if (newVal != value_)
	{
		value_ = newVal;
		if (trigger)
		{
			signalChanged_.emit(value_);
		}
		queue_draw();
	}
}

double CR42YDial::defaultValue()
{
	return defaultValue_;
}

void CR42YDial::setDefaultValue(double defaultValue)
{
	defaultValue_ = defaultValue;
}

std::string CR42YDial::text()
{
	return text_;
}

void CR42YDial::setText(std::string text)
{
	text_ = text;
	queue_draw();
}

sigc::signal<void, double> CR42YDial::signalChanged()
{
	return signalChanged_;
}

sigc::signal<void, double> CR42YDial::signalDone()
{
	return signalDone_;
}

bool CR42YDial::on_expose_event(GdkEventExpose* event)
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

		cr->arc(get_width() / 2, get_height() / 2, get_width() / 5 * 2, 2.45, 0.69);
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(get_width() / 20.);
		cr->stroke();

		cr->arc(get_width() / 2, get_height() / 2, get_width() / 5 * 2, 2.45, 2.45 + value_ * 4.52);
		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.8);
		cr->set_line_width(get_width() / 7.);
		cr->stroke();

		if (text_.size() > 0)
		{
			Cairo::TextExtents xtents;
			cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
			cr->set_font_size(tm->fontSizeSmall());
			cr->get_text_extents(text_, xtents);

			cr->move_to(get_width() / 2 - xtents.width / 2, get_height() - tm->fontSizeSmall() / 2);
			cr->show_text(text_);
		}
	}
}

void CR42YDial::on_realize()
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

bool CR42YDial::on_button_press(GdkEventButton* event)
{
	if (get_state() == Gtk::STATE_INSENSITIVE)
	{
		return false;
	}
	if (event->button == 1 && event->type == GDK_BUTTON_PRESS)
	{
		prePreClickValue_ = preClickValue_;
		preClickValue_ = value();
		//setValue(1 - event->y / get_height());
		mouseY_ = event->y;
		gtk_grab_add(gobj());
		return true;
	}
	else if (event->button == 1 && event->type == GDK_2BUTTON_PRESS)
	{
		if (fabs(prePreClickValue_ - defaultValue_) < 0.0001)
		{
			setValue(oldValue_);
		}
		else
		{
			oldValue_ = prePreClickValue_;
			setValue(defaultValue_);
		}
		return true;
	}
	else if (event->button == 3)
	{
		if (fabs(value() - defaultValue_) < 0.0001)
		{
			setValue(oldValue_);
		}
		else
		{
			oldValue_ = value();
			setValue(defaultValue_);
		}
		signalDone_.emit(value_);
		return true;
	}
	return false;
}

bool CR42YDial::on_button_release(GdkEventButton* event)
{
	if (event->button == 1)
	{
		gtk_grab_remove(gobj());
		signalDone_.emit(value_);
		return true;
	}
	return false;
}

bool CR42YDial::on_motion_notify(GdkEventMotion* event)
{
	if (get_state() != Gtk::STATE_INSENSITIVE)
	{
		//setValue(1 - event->y / get_height());
		float ydiff = mouseY_ - event->y;
		setValue(value_ + ydiff / get_height() / 2);
		mouseY_ = event->y;
		return true;
	}
	return false;
}

} /* namespace cr42y */
