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
