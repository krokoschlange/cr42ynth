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
class CR42YEntry;

class CR42YIntegerEntry : public CR42YRelativeContainer
{
public:
	CR42YIntegerEntry(CR42YUI* ui);
	virtual ~CR42YIntegerEntry();
	
	void setValue(int value);
	int value();
	
private:
	CR42YButton* increaseBtn_;
	CR42YButton* decreaseBtn_;
	CR42YEntry* entry_;
	
	int value_;
	int min_;
	int max_;
	
	void editCallback();
	void valueChangeCallback();
	void increaseCallback();
	void decreaseCallback();
	
	bool onButtonPress(GdkEventButton* event);
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YINTEGERENTRY_H_ */
