/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 * 3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior
 *     written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/
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
#include "CR42YToggleSelector.h"
#include "CR42YButton.h"
#include "CR42YToggle.h"

namespace cr42y
{

CR42YWFPartEditor::CR42YWFPartEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWFPartEditor"),
		CR42YRelativeContainer(ui),
		controller_(nullptr),
		wfPartPosLabel_(new CR42YLabel(ui)),
		wfPartPosEditor_(new CR42YIntegerEditor(ui)),
		typeSelector_(new CR42YToggleSelector(ui)),
		currentType_(0),
		wfFunctionGroup_(new CR42YRelativeContainer(ui)),
		wfFunctionLabel_(new CR42YLabel(ui)),
		wfFunctionEditor_(new CR42YEntry(ui)),
		wfHarmonicsGroup_(new CR42YRelativeContainer(ui)),
		wfHarmonicsSelector_(new CR42YToggleSelector(ui)),
		wfSamplesGroup_(new CR42YRelativeContainer(ui)),
		wfSamplesEditTgl_(new CR42YToggle(ui))
{
	setDrawBorder(true);

	wfPartPosLabel_->setText("POS");
	wfPartPosEditor_->setMin(0, true);
	wfPartPosEditor_->setMax(0, true);
	wfPartPosEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YWFPartEditor::posEditCallback));

	CR42YToggle* toggle = new CR42YToggle(ui);
	toggle->setText("SMPLS");
	typeSelector_->putToggle(toggle, 0.666, 0, 0.334, 1, 0, 2, 2, 2);
	toggle = new CR42YToggle(ui);
	toggle->setText("FUNC");
	typeSelector_->putToggle(toggle, 0, 0, 0.334, 1, 2, 2, 0, 2);
	toggle = new CR42YToggle(ui);
	toggle->setText("HARM");
	typeSelector_->putToggle(toggle, 0.333, 0, 0.334, 1, 0, 2, 0, 2);

	typeSelector_->signalSelected().connect(sigc::mem_fun(this, &CR42YWFPartEditor::typeSelectCallback));

	wfFunctionLabel_->setText("f(x, y) = ");
	wfFunctionEditor_->signal_changed().connect(sigc::mem_fun(this, &CR42YWFPartEditor::functionEditCallback));

	wfFunctionGroup_->put(wfFunctionLabel_, 0, 0, 0.3, 1, 2, 2, 2, 2);
	wfFunctionGroup_->put(wfFunctionEditor_, 0.3, 0, 0.7, 1, 2, 2, 2, 2);

	toggle = new CR42YToggle(ui);
	toggle->setText("SIN");
	wfHarmonicsSelector_->putToggle(toggle, 0, 0, 0.333, 0.5, 2, 2, 0, 0);
	toggle = new CR42YToggle(ui);
	toggle->setText("TRI");
	wfHarmonicsSelector_->putToggle(toggle, 0.333, 0, 0.333, 0.5, 0, 2, 0, 0);
	toggle = new CR42YToggle(ui);
	toggle->setText("SAW");
	wfHarmonicsSelector_->putToggle(toggle, 0.666, 0, 0.333, 0.5, 0, 2, 2, 0);
	toggle = new CR42YToggle(ui);
	toggle->setText("SQR");
	wfHarmonicsSelector_->putToggle(toggle, 0, 0.5, 0.333, 0.5, 2, 0, 0, 2);
	wfHarmonicsSelector_->signalSelected().connect(sigc::mem_fun(this, &CR42YWFPartEditor::harmSelectCallback));

	wfHarmonicsGroup_->put(wfHarmonicsSelector_, 0, 0, 1, 1, 2, 2, 2, 2);

	wfSamplesEditTgl_->setText("EDIT");
	wfSamplesEditTgl_->signalClicked().connect(sigc::mem_fun(this, &CR42YWFPartEditor::sampleEditCallback));

	wfSamplesGroup_->put(wfSamplesEditTgl_, 0, 0, 0.5, 0.5, 2, 2, 2, 2);

	put(wfPartPosLabel_, 0, 0, 0.2, 0.15, 2, 2, 2, 2);
	put(wfPartPosEditor_, 0.2, 0, 0.8, 0.15, 2, 2, 2, 2);

	put(typeSelector_, 0, 0.15, 1, 0.15, 2, 2, 2, 2);
	put(wfFunctionGroup_, 0, 0.3, 1, 0.2, 2, 2, 2, 2);
	put(wfHarmonicsGroup_, 0, 0.3, 1, 0.4, 2, 2, 2, 2);
	put(wfSamplesGroup_, 0, 0.3, 1, 0.3, 2, 2, 2, 2);
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
		int newType = controller_->getPartType();
		if (newType != currentType_)
		{
			currentType_ = newType;
			typeSelector_->select(currentType_, false);

			wfFunctionGroup_->hide_all();
			wfHarmonicsGroup_->hide_all();
			wfSamplesGroup_->hide_all();

			switch (newType)
			{
			case 0:
				wfSamplesGroup_->show_all();
				break;
			case 1:
				wfFunctionGroup_->show_all();
				break;
			case 2:
				wfHarmonicsGroup_->show_all();
				break;
			default:
				break;
			}
		}

		switch (currentType_)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			std::string func;
			controller_->getFunction(&func);
			if (!ignoreTextUpdate_)
			{
				wfFunctionEditor_->set_text(func);
			}
			else
			{
				ignoreTextUpdate_ = false;
			}
			break;
		}

		case 2:
		{
			int type = controller_->getHarmonicsType();
			if (type >= 0)
			{
				wfHarmonicsSelector_->select(type, false);
			}
			break;
		}
		default:
			break;
		}

		wfPartPosEditor_->setMin(0);
		wfPartPosEditor_->setMax(controller_->getPartAmount() - 1);
		wfPartPosEditor_->setValue(controller_->getSelectedPart());

		wfSamplesEditTgl_->set_state(controller_->getEditSelected() ? Gtk::STATE_ACTIVE : Gtk::STATE_NORMAL);
	}
}

void CR42YWFPartEditor::typeSelectCallback(int selected)
{
	if (controller_ && controller_->getSelectedPart() >= 0)
	{
		switch (selected)
		{
		case 0:
			controller_->replacePartWithDefault(WaveformPart::SAMPLES);
			break;
		case 1:
			controller_->replacePartWithDefault(WaveformPart::FUNCTION);
			break;
		case 2:
			controller_->replacePartWithDefault(WaveformPart::HARMONICS);
			break;
		default:
			break;
		}
	}
	else
	{
		typeSelector_->select(-1, false);
	}
}

void CR42YWFPartEditor::posEditCallback(int newPos)
{
	if (controller_)
	{
		controller_->movePart(controller_->getSelectedPart(), newPos);
		controller_->selectPart(newPos);
	}
}

void CR42YWFPartEditor::functionEditCallback()
{
	if (controller_)
	{
		ignoreTextUpdate_ = true;
		Glib::ustring txt = wfFunctionEditor_->get_text();
		controller_->setFunction(txt);
	}
}

void CR42YWFPartEditor::harmSelectCallback(int selected)
{
	if (controller_)
	{
		controller_->setHarmonicsType((WPHarmonics::functionType) selected);
	}
}

void CR42YWFPartEditor::sampleEditCallback()
{
	if (controller_)
	{
		if (wfSamplesEditTgl_->get_state() == Gtk::STATE_ACTIVE)
		{
			controller_->setEditSelected(true);
		}
		else
		{
			controller_->setEditSelected(false);
		}
	}
}

} /* namespace cr42y */
