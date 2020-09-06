/*
 * CR42YWFPartEditor.h
 *
 *  Created on: 05.09.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWFPARTEDITOR_H_
#define SRC_UI_CR42YWFPARTEDITOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class WavetableEditController;
class CR42YLabel;
class CR42YEntry;
class CR42YIntegerEditor;

class CR42YWFPartEditor : public CR42YRelativeContainer
{
public:
	CR42YWFPartEditor(CR42YUI* ui);
	virtual ~CR42YWFPartEditor();

	void setController(WavetableEditController* controller);

	void update();

private:
	WavetableEditController* controller_;

	CR42YLabel* wfPartPosLabel_;
	CR42YIntegerEditor* wfPartPosEditor_;

	CR42YLabel* wfFunctionLabel_;
	CR42YEntry* wfFunctionEditor_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWFPARTEDITOR_H_ */
