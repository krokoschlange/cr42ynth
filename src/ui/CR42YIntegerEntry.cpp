/*
 * CR42YIntegerEntry.cpp
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#include "CR42YIntegerEntry.h"

#include "CR42YButton.h"
#include "CR42YTheme.h"
#include "helpers.h"

#include <iostream>

namespace cr42y
{

CR42YIntegerEntry::CR42YIntegerEntry(CR42YUI* ui) :
		Glib::ObjectBase("CR42YIntegerEntry"),
		CR42YLabel(ui),
		value_(0),
		min_(0),
		max_(8),
		useMin_(true),
		useMax_(true)
{
	setText(std::to_string(value_));
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::KEY_PRESS_MASK);
	set_can_focus();
}

CR42YIntegerEntry::~CR42YIntegerEntry()
{
}

void CR42YIntegerEntry::setValue(int value)
{
	int old = value_;
	if (useMin_)
	{
		value_ = std::max<int>(min_, value);
	}
	if (useMax_)
	{
		value_ = std::min<int>(max_, value_);
	}
	setText(std::to_string(value_));
	queue_draw();
	if (old != value_)
	{
		signalChanged_.emit(value_);
	}
}

int CR42YIntegerEntry::value()
{
	return value_;
}

void CR42YIntegerEntry::setMin(int min, bool use)
{
	min_ = min;
	useMin_ = use;
}

int CR42YIntegerEntry::min()
{
	return min_;
}

void CR42YIntegerEntry::setMax(int max, bool use)
{
	max_ = max;
	useMax_ = use;
}

int CR42YIntegerEntry::max()
{
	return max_;
}

sigc::signal<void, int> CR42YIntegerEntry::signalChanged()
{
	return signalChanged_;
}

bool CR42YIntegerEntry::on_button_press_event(GdkEventButton* event)
{
	if (event->button == 1 && event->x > get_width() * 0.25 && event->x < get_width() * 0.75)
	{
		setValue(0);
		if (!has_focus())
		{
			grab_focus();
		}
		return true;
	}
	return false;
}

bool CR42YIntegerEntry::on_key_press_event(GdkEventKey* event)
{
	int keys[] =
		{GDK_0, GDK_KP_0, GDK_1, GDK_KP_1, GDK_2, GDK_KP_2, GDK_3, GDK_KP_3,
		GDK_4, GDK_KP_4, GDK_5, GDK_KP_5, GDK_6, GDK_KP_6, GDK_7,
		GDK_KP_7, GDK_8, GDK_KP_8, GDK_9, GDK_KP_9};
	for (int k = 0; k < 10; k++)
	{
		if (event->keyval == keys[k * 2] || event->keyval == keys[k * 2 + 1])
		{
			if (value() < 0)
			{
				setValue(value() * 10 - k);
			}
			else
			{
				setValue(value() * 10 + k);
			}
			return true;
		}
	}
	if (event->keyval == GDK_minus || event->keyval == GDK_KP_Subtract)
	{
		setValue(value() * -1);
		return true;
	}
	if (event->keyval == GDK_BackSpace)
	{
		setValue(value() / 10);
		return true;
	}
	return false;
}

} /* namespace cr42y */
