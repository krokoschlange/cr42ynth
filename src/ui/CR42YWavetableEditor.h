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
class CR42YWaveformEditorControlPanel;
class CR42YHarmonicsView;
class CR42YWavetableView;
class CR42YButton;
class CR42YHarmonicsEditor;

class CR42YWavetableEditor : public CR42YRelativeContainer
{
public:
	CR42YWavetableEditor(CR42YUI* ui);
	virtual ~CR42YWavetableEditor();

private:
	WavetableEditController* controller_;

	CR42YWaveformEditor* wfEditor_;
	CR42YWaveformEditorControlPanel* wfeControllerPanel_;
	CR42YHarmonicsView* harmonicsView_;
	CR42YWavetableView* wtView_;
	CR42YHarmonicsEditor* harmonicsEditor_;
	CR42YButton* addWaveformButton_;

	void waveformAddCallback();

	void waveformUpdateCallback();
	void wavetableUpdateCallback();

	void partSelectCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEEDITOR_H_ */
