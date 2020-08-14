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

class CR42YWaveformEditorControlPanel : public CR42YRelativeContainer
{
public:
	CR42YWaveformEditorControlPanel(CR42YUI* ui);
	virtual ~CR42YWaveformEditorControlPanel();

private:
	CR42YButton* toSinBtn_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_ */
