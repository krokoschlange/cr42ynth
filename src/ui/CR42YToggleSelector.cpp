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
/*
 * CR42YToggleSelector.cpp
 *
 *  Created on: 07.09.2020
 *      Author: fabian
 */

#include "CR42YToggleSelector.h"
#include "CR42YToggle.h"

namespace cr42y
{

CR42YToggleSelector::CR42YToggleSelector(CR42YUI* ui) :
		Glib::ObjectBase("CR42YToggleSelector"),
		CR42YRelativeContainer(ui),
		selected_(-1)
{
	setDrawBorder(true);
}

CR42YToggleSelector::~CR42YToggleSelector()
{
	for (int i = 0; i < toggles_.size(); i++)
	{
		toggles_[i].second.disconnect();
		remove(*(toggles_[i].first));
		if (toggles_[i].first)
		{
			delete toggles_[i].first;
		}
	}
}

void CR42YToggleSelector::select(int selected, bool callback)
{
	int old = selected_;
	selected_ = selected;
	for (int i = 0; i < toggles_.size(); i++)
	{
		if (i == selected)
		{
			toggles_[i].first->set_state(Gtk::STATE_ACTIVE);
		}
		else
		{
			toggles_[i].first->set_state(Gtk::STATE_NORMAL);
		}
	}

	if (old != selected_ && callback)
	{
		signalSelected_.emit(selected_);
	}
}

int CR42YToggleSelector::selected()
{
	return selected_;
}

void CR42YToggleSelector::putToggle(CR42YToggle* toggle, double x, double y,
		double w, double h, int padLeft, int padTop, int padRight,
		int padBottom)
{
	put(toggle, x, y, w, h, padLeft, padTop, padRight, padBottom);
	sigc::connection conn = toggle->signalClicked().connect(sigc::bind<int, bool>(sigc::mem_fun(this, &CR42YToggleSelector::select), toggles_.size(), true));
	toggles_.push_back(std::pair<CR42YToggle*, sigc::connection>(toggle, conn));
}

void CR42YToggleSelector::removeToggle(CR42YToggle* toggle)
{
	for (int i = 0; i < toggles_.size(); i++)
	{
		if (toggles_[i].first == toggle)
		{
			toggles_[i].second.disconnect();
			toggles_.erase(toggles_.begin() + i);
			i--;
		}
	}
	remove(*toggle);
}

sigc::signal<void, int> CR42YToggleSelector::signalSelected()
{
	return signalSelected_;
}

} /* namespace cr42y */
