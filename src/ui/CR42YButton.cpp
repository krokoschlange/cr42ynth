/*
 * CR42YButton.cpp
 *
 *  Created on: 25.06.2020
 *      Author: fabian
 */

#include "CR42YButton.h"

#include <iostream>

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
