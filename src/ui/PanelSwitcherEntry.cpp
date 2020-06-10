/*
 * PanelSwitcherEntry.cpp
 *
 *  Created on: 19.05.2020
 *      Author: fabian
 */

#include "PanelSwitcherEntry.h"

namespace cr42y
{

PanelSwitcherEntry::PanelSwitcherEntry(Avtk::Button* btn, Avtk::Widget* pnl) :
		button(btn),
		panel(pnl)
{
}

PanelSwitcherEntry::~PanelSwitcherEntry()
{
}

} /* namespace cr42y */
