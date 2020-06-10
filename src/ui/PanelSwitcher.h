/*
 * PanelSwitcher.h
 *
 *  Created on: 19.05.2020
 *      Author: fabian
 */

#ifndef SRC_UI_PANELSWITCHER_H_
#define SRC_UI_PANELSWITCHER_H_

#include <vector>

#include "group.hxx"

namespace Avtk
{
class Button;
}

namespace cr42y
{

class PanelSwitcherEntry;

class PanelSwitcher : public Avtk::Group
{
public:
	PanelSwitcher(Avtk::UI* ui, int x, int y, int w, int h);
	virtual ~PanelSwitcher();

	void addPanel(Avtk::Button* button, Avtk::Group* panel);

	void select(int p);

	virtual void valueCB(Avtk::Widget* widget);

private:
	std::vector<PanelSwitcherEntry*> panels;
};

} /* namespace cr42y */

#endif /* SRC_UI_PANELSWITCHER_H_ */
