/*
 * CR42YWavetableEditor.h
 *
 *  Created on: 05.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWAVETABLEEDITOR_H_
#define SRC_UI_CR42YWAVETABLEEDITOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class WavetableEditController;
class CR42YWaveformEditor;
class CR42YHarmonicsView;
class CR42YWavetableView;
class CR42YButton;

class CR42YWavetableEditor : public CR42YRelativeContainer
{
public:
	CR42YWavetableEditor(CR42YUI* ui);
	virtual ~CR42YWavetableEditor();

private:
	WavetableEditController* controller_;

	CR42YWaveformEditor* wfEditor_;
	CR42YHarmonicsView* harmonicsView_;
	CR42YWavetableView* wtView_;
	CR42YButton* addWaveformButton_;

	void waveformUpdateCallback();
	void waveformAddCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEEDITOR_H_ */
