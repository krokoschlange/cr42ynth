/*
 * CR42YToggle.cpp
 *
 *  Created on: 03.09.2020
 *      Author: fabian
 */

#include "CR42YToggle.h"

namespace cr42y
{

CR42YToggle::CR42YToggle(CR42YUI* ui) :
		Glib::ObjectBase("CR42YToggle"),
		CR42YLabel(ui)
{
	Gtk::Widget::add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YToggle::on_button_press));
	set_can_focus();
}

CR42YToggle::~CR42YToggle()
{
}

sigc::signal<void> CR42YToggle::signalClicked()
{
	return signalClicked_;
}

bool CR42YToggle::on_button_press(GdkEventButton* event)
{
	if (get_state() == Gtk::STATE_ACTIVE)
	{
		set_state(Gtk::STATE_NORMAL);
	}
	else
	{
		set_state(Gtk::STATE_ACTIVE);
	}
	if (!has_focus())
	{
		grab_focus();
	}
	signalClicked_.emit();
	return true;
}

} /* namespace cr42y */
