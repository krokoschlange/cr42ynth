/*
 * CR42YWaveformEditorControlPanel.h
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_
#define SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class CR42YButton;
class CR42YLabel;
class CR42YIntegerEditor;
class CR42YWaveformEditor;
class WavetableEditController;

class CR42YWaveformEditorControlPanel : public CR42YRelativeContainer
{
public:
	CR42YWaveformEditorControlPanel(CR42YUI* ui, CR42YWaveformEditor* wfEditor, WavetableEditController* controller);
	virtual ~CR42YWaveformEditorControlPanel();

	void gridXCallback(int val);
	void gridYCallback(int val);
	void toSinCallback();

private:
	CR42YWaveformEditor* wfEditor_;
	WavetableEditController* controller_;

	CR42YLabel* gridXLabel_;
	CR42YIntegerEditor* gridXEditor_;
	CR42YLabel* gridYLabel_;
	CR42YIntegerEditor* gridYEditor_;
	CR42YButton* toSinBtn_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_ */
