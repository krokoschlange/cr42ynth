/*
 * CR42YHarmonicsEditor.h
 *
 *  Created on: 31.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YHARMONICSEDITOR_H_
#define SRC_UI_CR42YHARMONICSEDITOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{
class WavetableEditController;
class CR42YBoxVScale;

class CR42YHarmonicsEditor : public CR42YRelativeContainer
{
public:
	CR42YHarmonicsEditor(CR42YUI* ui);
	virtual ~CR42YHarmonicsEditor();

	void setController(WavetableEditController* controller);

	void update();
	void setHarmonicsTable(std::vector<std::pair<float, float>>* harmTable);

protected:
	bool on_expose_event(GdkEventExpose* event);

private:
	std::vector<CR42YBoxVScale*> ampScales_;
	std::vector<CR42YBoxVScale*> phaseScales_;

	WavetableEditController* controller_;
	std::vector<std::pair<float, float>>* harmTable_;
	int columnWidth_;

	void changedCallback(double value, int column, bool isAmp);
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YHARMONICSEDITOR_H_ */
