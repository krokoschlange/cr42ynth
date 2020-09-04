/*
 * CR42YWaveformEditorControlPanel.cpp
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#include "CR42YWaveformEditorControlPanel.h"
#include "CR42YLabel.h"
#include "CR42YButton.h"
#include "CR42YIntegerEditor.h"
#include "CR42YWaveformEditor.h"
#include "WavetableEditController.h"

namespace cr42y
{

CR42YWaveformEditorControlPanel::CR42YWaveformEditorControlPanel(CR42YUI* ui,
		CR42YWaveformEditor* wfEditor, WavetableEditController* controller) :
		Glib::ObjectBase("CR42YWaveformEditorControlPanel"),
		wfEditor_(wfEditor),
		controller_(controller),
		CR42YRelativeContainer(ui),
		gridXLabel_(new CR42YLabel(ui)),
		gridXEditor_(new CR42YIntegerEditor(ui)),
		gridYLabel_(new CR42YLabel(ui)),
		gridYEditor_(new CR42YIntegerEditor(ui)),
		toSinBtn_(new CR42YButton(ui)),
		normalizeBtn_(new CR42YButton(ui))
{
	setDrawBorder(true);

	gridXLabel_->setText("Grid X");
	gridXEditor_->setMin(0);
	gridXEditor_->setMax(0, false);
	gridYLabel_->setText("Grid Y");
	gridYEditor_->setMin(0);
	gridYEditor_->setMax(0, false);

	toSinBtn_->setText("TO SIN");
	normalizeBtn_->setText("NORMALIZE");

	gridXEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YWaveformEditorControlPanel::gridXCallback));
	gridYEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YWaveformEditorControlPanel::gridYCallback));
	toSinBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YWaveformEditorControlPanel::toSinCallback));
	normalizeBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YWaveformEditorControlPanel::normalizeCallback));

	put(gridXLabel_, 0, 0, 97, 1, 2, 2, 0, 2);
	put(gridXEditor_, 97, 0, 100, 1, 5, 2, 0, 2);
	put(gridYLabel_, 197, 0, 100, 1, 5, 2, 0, 2);
	put(gridYEditor_, 297, 0, 100, 1, 5, 2, 0, 2);
	put(toSinBtn_, 397, 0, 100, 1, 5, 2, 0, 2);
	put(normalizeBtn_, 497, 0, 100, 1, 5, 2, 0, 2);
}

CR42YWaveformEditorControlPanel::~CR42YWaveformEditorControlPanel()
{
}

void CR42YWaveformEditorControlPanel::gridXCallback(int val)
{
	controller_->setGridX(val);
	wfEditor_->queue_draw();
}

void CR42YWaveformEditorControlPanel::gridYCallback(int val)
{
	controller_->setGridY(val);
	wfEditor_->queue_draw();
}

void CR42YWaveformEditorControlPanel::toSinCallback()
{
	controller_->convertToSin();
}

void CR42YWaveformEditorControlPanel::normalizeCallback()
{
	controller_->normalizeHarmonic();
}

} /* namespace cr42y */
