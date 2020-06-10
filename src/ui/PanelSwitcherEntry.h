/*
 * PanelSwitcherEntry.h
 *
 *  Created on: 19.05.2020
 *      Author: fabian
 */

#ifndef SRC_UI_PANELSWITCHERENTRY_H_
#define SRC_UI_PANELSWITCHERENTRY_H_

namespace Avtk
{
class Widget;
class Button;
}

namespace cr42y
{

class PanelSwitcherEntry
{
public:
	PanelSwitcherEntry(Avtk::Button* btn, Avtk::Widget* pnl);
	virtual ~PanelSwitcherEntry();

	Avtk::Widget* panel;
	Avtk::Button* button;
};

} /* namespace cr42y */

#endif /* SRC_UI_PANELSWITCHERENTRY_H_ */
