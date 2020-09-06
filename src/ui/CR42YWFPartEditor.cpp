/*
 * CR42YWFPartEditor.cpp
 *
 *  Created on: 05.09.2020
 *      Author: fabian
 */

#include "CR42YWFPartEditor.h"

#include "WavetableEditController.h"
#include "CR42YLabel.h"
#include "CR42YIntegerEditor.h"
#include "CR42YEntry.h"

namespace cr42y
{

CR42YWFPartEditor::CR42YWFPartEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWFPartEditor"),
		CR42YRelativeContainer(ui),
		controller_(nullptr),
		wfPartPosLabel_(new CR42YLabel(ui)),
		wfPartPosEditor_(new CR42YIntegerEditor(ui)),
		wfFunctionLabel_(new CR42YLabel(ui)),
		wfFunctionEditor_(new CR42YEntry(ui))
{
	wfPartPosLabel_->setText("POS");
	wfPartPosEditor_->setMin(0, true);
	wfPartPosEditor_->setMax(0, true);

	wfFunctionLabel_->setText("f(x, y) = ");
	wfFunctionEditor_->set_sensitive(false);

	put(wfPartPosLabel_, 0, 0, 0.2, 0.5, 2, 2, 2, 2);
	put(wfPartPosEditor_, 0.2, 0, 0.8, 0.5, 2, 2, 2, 2);

	put(wfFunctionLabel_, 0, 0.5, 0.3, 0.5, 2, 2, 2, 2);
	put(wfFunctionEditor_, 0.3, 0.5, 0.7, 0.5, 2, 2, 2, 2);
}

CR42YWFPartEditor::~CR42YWFPartEditor()
{
	delete wfPartPosLabel_;
	delete wfPartPosEditor_;
	delete wfFunctionLabel_;
	delete wfFunctionEditor_;
}

void CR42YWFPartEditor::setController(WavetableEditController* controller)
{
	controller_ = controller;
}

void CR42YWFPartEditor::update()
{
	if (controller_)
	{
		controller_->get
	}
}

} /* namespace cr42y */
