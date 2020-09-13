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
