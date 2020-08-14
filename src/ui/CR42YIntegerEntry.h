/*
 * CR42YIntegerEntry.h
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YINTEGERENTRY_H_
#define SRC_UI_CR42YINTEGERENTRY_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{
class CR42YButton;

class CR42YIntegerEntry : public CR42YRelativeContainer
{
public:
	CR42YIntegerEntry(CR42YUI* ui);
	virtual ~CR42YIntegerEntry();

private:
	CR42YButton* increaseBtn_;
	CR42YButton* decreaseBtn_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YINTEGERENTRY_H_ */
