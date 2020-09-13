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
class CR42YToggleSelector;
class CR42YToggle;

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

	CR42YToggleSelector* typeSelector_;

	int currentType_;

	CR42YRelativeContainer* wfFunctionGroup_;
	CR42YLabel* wfFunctionLabel_;
	CR42YEntry* wfFunctionEditor_;

	CR42YRelativeContainer* wfHarmonicsGroup_;
	CR42YToggleSelector* wfHarmonicsSelector_;

	CR42YRelativeContainer* wfSamplesGroup_;
	CR42YToggle* wfSamplesEditTgl_;

	bool ignoreTextUpdate_;

	void typeSelectCallback(int selected);
	void posEditCallback(int newPos);
	void functionEditCallback();
	void harmSelectCallback(int selected);
	void sampleEditCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWFPARTEDITOR_H_ */
