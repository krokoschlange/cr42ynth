/*
 * CR42YIntegerEditor.h
 *
 *  Created on: 29.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YINTEGEREDITOR_H_
#define SRC_UI_CR42YINTEGEREDITOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class CR42YButton;
class CR42YIntegerEntry;

class CR42YIntegerEditor : public CR42YRelativeContainer
{
public:
	CR42YIntegerEditor(CR42YUI* ui);
	virtual ~CR42YIntegerEditor();

	void setValue(int value);
	int value();
	void setMin(int min, bool use = true);
	int min();
	void setMax(int max, bool use = true);
	int max();

	sigc::signal<void, int> signalChanged();

private:
	CR42YButton* increaseBtn_;
	CR42YButton* decreaseBtn_;
	CR42YIntegerEntry* entry_;

	void increaseCallback();
	void decreaseCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YINTEGEREDITOR_H_ */
