/*
 * CR42YToolSelector.h
 *
 *  Created on: 03.09.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YTOOLSELECTOR_H_
#define SRC_UI_CR42YTOOLSELECTOR_H_

#include "CR42YToggleSelector.h"

namespace cr42y
{

class WavetableEditController;

class CR42YToolSelector : public CR42YToggleSelector
{
public:
	CR42YToolSelector(CR42YUI* ui, WavetableEditController* controller);
	virtual ~CR42YToolSelector();

	void selectCallback(int selected);

private:
	std::vector<int> mapping_;
	WavetableEditController* controller_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YTOOLSELECTOR_H_ */
