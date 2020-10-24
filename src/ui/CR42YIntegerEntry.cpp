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

#include "CR42YIntegerEntry.h"

#include "CR42YButton.h"
#include "CR42YTheme.h"
#include "helpers.h"

namespace cr42y
{

CR42YIntegerEntry::CR42YIntegerEntry(CR42YUI* ui) :
		Glib::ObjectBase("CR42YIntegerEntry"),
		CR42YLabel(ui),
		value_(0),
		showValue_(0),
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
	setShowValue(value_);
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
		setShowValue(0);
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
			int val = 0;
			if (showValue_ < 0)
			{
				val = showValue_ * 10 - k;
			}
			else
			{
				val = showValue_ * 10 + k;
			}
			setValue(val);
			setShowValue(val);
			return true;
		}
	}
	if (event->keyval == GDK_minus || event->keyval == GDK_KP_Subtract)
	{
		setValue(showValue_ * -1);
		setShowValue(showValue_ * -1);
		return true;
	}
	if (event->keyval == GDK_BackSpace)
	{
		setValue(showValue_ / 10);
		setShowValue(showValue_ / 10);
		return true;
	}
	if (event->keyval == GDK_Return)
	{
		setValue(showValue_);
		return true;
	}
	return false;
}

bool CR42YIntegerEntry::on_focus_out_event(GdkEventFocus* event)
{
	setValue(showValue_);
}

void CR42YIntegerEntry::setShowValue(int showValue)
{
	showValue_ = showValue;
	setText(std::to_string(showValue));
	queue_draw();
}

} /* namespace cr42y */
