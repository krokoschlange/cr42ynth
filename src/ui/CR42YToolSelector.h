/*
 * CR42YToolSelector.h
 *
 *  Created on: 03.09.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YTOOLSELECTOR_H_
#define SRC_UI_CR42YTOOLSELECTOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class WavetableEditController;
class CR42YToggle;

class CR42YToolSelector : public CR42YRelativeContainer
{
public:
	CR42YToolSelector(CR42YUI* ui, WavetableEditController* controller);
	virtual ~CR42YToolSelector();

	void selectCallback(CR42YToggle* source, int tool);

private:
	WavetableEditController* controller_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YTOOLSELECTOR_H_ */
