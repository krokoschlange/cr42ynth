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
#include "CR42YControlToggle.h"

namespace cr42y
{

CR42YControlToggle::CR42YControlToggle(CR42YUI* ui) :
		CR42YToggle(ui)
{
	using std::placeholders::_1;
	connector_.setWidgetValueSetter(std::bind(&CR42YControlToggle::setValue, this, _1));
	signalClicked().connect(
			sigc::mem_fun(this, &CR42YControlToggle::clickedCallback));
}

CR42YControlToggle::~CR42YControlToggle()
{
}

void CR42YControlToggle::connectControl(Control* control)
{
	if (control)
	{
		connector_.connect(*control);
		setValue(connector_.getControl()->getValue());
	}
	else
	{
		setValue(0);
	}
}

void CR42YControlToggle::setValue(double value)
{
	if (value >= 0.5)
	{
		set_state(Gtk::STATE_ACTIVE);
	}
	else
	{
		set_state(Gtk::STATE_NORMAL);
	}
}

double CR42YControlToggle::value()
{
	return get_state() == Gtk::STATE_ACTIVE;
}

void CR42YControlToggle::clickedCallback()
{
	connector_.setControlValue(value());
}

} /* namespace cr42y */
