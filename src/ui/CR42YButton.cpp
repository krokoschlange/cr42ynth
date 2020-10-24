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

#include "CR42YButton.h"

#include <cairomm/context.h>
#include "helpers.h"

#include <sigc++/sigc++.h>

namespace cr42y
{

CR42YButton::CR42YButton(CR42YUI* ui) :
		Glib::ObjectBase("CR42YButton"),
		CR42YLabel(ui),
		autorepeat_(false)
{
	Gtk::Widget::add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YButton::on_button_press));
	signal_button_release_event().connect(sigc::mem_fun(this, &CR42YButton::on_button_release));
	set_can_focus();
}

CR42YButton::~CR42YButton()
{

}

sigc::signal<void> CR42YButton::signalClicked()
{
	return signalClicked_;
}

void CR42YButton::setAutorepeat(bool autorepeat)
{
	autorepeat_ = autorepeat;
}

bool CR42YButton::on_button_press(GdkEventButton* event)
{
	if (get_state() == Gtk::STATE_INSENSITIVE)
	{
		return false;
	}
	set_state(Gtk::STATE_ACTIVE);
	if (!has_focus())
	{
		grab_focus();
	}
	if (autorepeat_)
	{
		Glib::signal_timeout().connect_once(sigc::mem_fun(this, &CR42YButton::timeout), 300);
	}
	return true;
}

bool CR42YButton::on_button_release(GdkEventButton* event)
{
	set_state(Gtk::STATE_NORMAL);
	if (event->x > 0 && event->x < get_width() && event->y > 0 && event->y < get_height())
	{
		signalClicked_.emit();
	}
	return true;
}

void CR42YButton::timeout()
{
	if (get_state() == Gtk::STATE_ACTIVE)
	{
		signalClicked_.emit();
		Glib::signal_timeout().connect_once(sigc::mem_fun(this, &CR42YButton::timeout), 100);
	}

}

} /* namespace cr42y */
