/*
 * CR42YToggleSelector.h
 *
 *  Created on: 07.09.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YTOGGLESELECTOR_H_
#define SRC_UI_CR42YTOGGLESELECTOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class CR42YToggle;

class CR42YToggleSelector : public CR42YRelativeContainer
{
public:
	CR42YToggleSelector(CR42YUI* ui);
	virtual ~CR42YToggleSelector();

	void select(int selected);
	int selected();

	void putToggle(CR42YToggle* toggle, double x, double y, double w, double h,
			int padLeft = 0, int padTop = 0, int padRight = 0,
			int padBottom = 0);

	void removeToggle(CR42YToggle* toggle);

	sigc::signal<void, int> signalSelected();

private:
	std::vector<std::pair<CR42YToggle*, sigc::connection>> toggles_;
	int selected_;
	sigc::signal<void, int> signalSelected_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YTOGGLESELECTOR_H_ */
