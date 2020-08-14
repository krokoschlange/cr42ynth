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
		CR42YLabel(ui)
{
	Gtk::Widget::add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YButton::on_button_press));
	signal_button_release_event().connect(sigc::mem_fun(this, &CR42YButton::on_button_release));
}

CR42YButton::~CR42YButton()
{

}

sigc::signal<void> CR42YButton::signalClicked()
{
	return signalClicked_;
}

bool CR42YButton::on_button_press(GdkEventButton* event)
{
	set_state(Gtk::STATE_ACTIVE);
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

} /* namespace cr42y */
