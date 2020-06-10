/*
 * PanelSwitcher.cpp
 *
 *  Created on: 19.05.2020
 *      Author: fabian
 */

#include "PanelSwitcher.h"

#include "avtk.hxx"

#include "PanelSwitcherEntry.h"

namespace cr42y
{

PanelSwitcher::PanelSwitcher(Avtk::UI* ui, int x, int y, int w, int h) :
		Avtk::Group(ui, x, y, w, h, ""),
		panels()
{

}

PanelSwitcher::~PanelSwitcher()
{
	for (int i = 0; i < panels.size(); i++)
	{
		delete panels[i];
	}
	panels.clear();
}

void PanelSwitcher::addPanel(Avtk::Button* button, Avtk::Group* panel)
{
	panels.push_back(new PanelSwitcherEntry(button, panel));
	add(button);
}

void PanelSwitcher::select(int p)
{
	for (int i = 0; i < panels.size(); i++)
	{
		if (i == p)
		{
			panels[i]->button->value(1);
			panels[i]->panel->visible(true);
			value(i);
			callback(this, callbackUD);
		}
		else
		{
			panels[i]->button->value(0);
			panels[i]->panel->visible(false);
		}
	}
}

void PanelSwitcher::valueCB(Avtk::Widget* widget)
{
	for (int i = 0; i < panels.size(); i++)
	{
		if (panels[i]->button == widget)
		{
			panels[i]->button->value(1);
			panels[i]->panel->visible(true);
			value(i);
			callback(this, callbackUD);
		}
		else
		{
			panels[i]->button->value(0);
			panels[i]->panel->visible(false);
		}
	}
}

} /* namespace cr42y */
