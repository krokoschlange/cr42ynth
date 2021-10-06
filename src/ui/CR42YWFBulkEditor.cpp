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

#include "CR42YWFBulkEditor.h"

#include "WavetableEditController.h"
#include "CR42YButton.h"
#include "CR42YEntry.h"
#include "CR42YIntegerEditor.h"
#include "CR42YToggleSelector.h"
#include "CR42YToggle.h"

#include "WPFunction.h"

namespace cr42y
{

CR42YWFBulkEditor::CR42YWFBulkEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWFBulkEditor"),
		CR42YGrid(ui),
		controller_(nullptr),
		opSelector_(new CR42YToggleSelector(ui)),
		addBtn_(new CR42YButton(ui)),
		mathGroup_(new CR42YGrid(ui)),
		mathFuncLabel_(new CR42YLabel(ui)),
		mathFuncEntry_(new CR42YEntry(ui)),
		mathStartLabel_(new CR42YLabel(ui)),
		mathStartEditor_(new CR42YIntegerEditor(ui)),
		mathAmntLabel_(new CR42YLabel(ui)),
		mathAmntEditor_(new CR42YIntegerEditor(ui)),
		wavGroup_(new CR42YGrid(ui)),
		wavFileEntry_(new CR42YEntry(ui)),
		wavFileBtn_(new CR42YButton(ui)),
		wavStartLabel_(new CR42YLabel(ui)),
		wavStartEditor_(new CR42YIntegerEditor(ui)),
		wavAmntTypeSelector_(new CR42YToggleSelector(ui)),
		wavAmntEditor_(new CR42YIntegerEditor(ui)),
		morphGroup_(new CR42YGrid(ui)),
		morphStartLabel_(new CR42YLabel(ui)),
		morphStartEditor_(new CR42YIntegerEditor(ui)),
		morphAmntLabel_(new CR42YLabel(ui)),
		morphAmntEditor_(new CR42YIntegerEditor(ui)),
		morphTypeSelector_(new CR42YToggleSelector(ui))
{
	setDrawBorder(true);

	configureColumn(0, 1, 0.003, 0.003, 0, 0);
	configureRow(0, 1, 0.003, 0.003, 0, 0);
	configureRow(1, 4, 0.003, 0.003, 0, 0);
	configureRow(2, 1, 0.003, 0.003, 0, 0);

	CR42YToggle* tgl = new CR42YToggle(ui);
	tgl->setText("MATH");
	opSelector_->putToggle(tgl, 0, 0, 0.333, 1, 0, 0, 0, 0);
	tgl = new CR42YToggle(ui);
	tgl->setText("WAV");
	opSelector_->putToggle(tgl, 0.333, 0, 0.333, 1, 0, 0, 0, 0);
	tgl = new CR42YToggle(ui);
	tgl->setText("MORPH");
	opSelector_->putToggle(tgl, 0.666, 0, 0.334, 1, 0, 0, 0, 0);

	opSelector_->select(0);

	opSelector_->signalSelected().connect(
			sigc::mem_fun(this, &CR42YWFBulkEditor::opSelectCallback));

	addBtn_->setText("ADD");
	addBtn_->signalClicked().connect(
			sigc::mem_fun(this, &CR42YWFBulkEditor::addCallback));

	mathFuncLabel_->setText("f(x, y)=");
	mathStartLabel_->setText("START");
	mathStartEditor_->setMin(0, true);
	mathAmntLabel_->setText("AMOUNT");
	mathAmntEditor_->setMin(1, true);
	mathAmntEditor_->setMax(1, false);
	mathAmntEditor_->setValue(1);

	mathGroup_->configureColumn(0, 2, 0, 0.003, 0, 0);
	mathGroup_->configureColumn(1, 3, 0.003, 0, 0, 0);

	mathGroup_->configureRow(0, 1, 0, 0.003, 0, 0);
	mathGroup_->configureRow(1, 1, 0.003, 0.003, 0, 0);
	mathGroup_->configureRow(2, 1, 0.003, 0, 0, 0);

	mathGroup_->put(mathFuncLabel_, 0, 0); //0, 0, 0.2, 0.4, 2, 2, 2, 2);
	mathGroup_->put(mathFuncEntry_, 0, 1); //0.2, 0, 0.8, 0.4, 2, 2, 2, 2);
	mathGroup_->put(mathStartLabel_, 1, 0); //0, 0.4, 0.4, 0.3, 2, 2, 2, 2);
	mathGroup_->put(mathStartEditor_, 1, 1); //0.4, 0.4, 0.6, 0.3, 2, 2, 2, 2);
	mathGroup_->put(mathAmntLabel_, 2, 0); //0, 0.7, 0.4, 0.3, 2, 2, 2, 2);
	mathGroup_->put(mathAmntEditor_, 2, 1); //0.4, 0.7, 0.6, 0.3, 2, 2, 2, 2);

	wavFileBtn_->setText("...");
	wavStartLabel_->setText("START");
	wavStartEditor_->setMin(0, true);

	wavFileBtn_->signalClicked().connect(
			sigc::mem_fun(this, &CR42YWFBulkEditor::fileChooseCallback));

	tgl = new CR42YToggle(ui);
	tgl->setText("AMOUNT");
	wavAmntTypeSelector_->putToggle(tgl, 0, 0, 0.5, 1, 0, 0, 0, 0);
	tgl = new CR42YToggle(ui);
	tgl->setText("WIDTH");
	wavAmntTypeSelector_->putToggle(tgl, 0.5, 0, 0.5, 1, 0, 0, 0, 0);

	wavAmntTypeSelector_->select(1);

	wavAmntEditor_->setMin(1, true);
	wavAmntEditor_->setMax(1, false);
	wavAmntEditor_->setValue(2048);

	wavGroup_->configureColumn(0, 2, 0, 0.003, 0, 0);
	wavGroup_->configureColumn(1, 2, 0.003, 0.003, 0, 0);
	wavGroup_->configureColumn(2, 1, 0.003, 0, 0, 0);

	wavGroup_->configureRow(0, 1, 0, 0.003, 0, 0);
	wavGroup_->configureRow(1, 1, 0.003, 0.003, 0, 0);
	wavGroup_->configureRow(2, 1, 0.003, 0.003, 0, 0);
	wavGroup_->configureRow(3, 1, 0.003, 0, 0, 0);

	wavGroup_->put(wavFileEntry_, 0, 0, 1, 2); //0, 0, 0.8, 0.25, 2, 2, 2, 2);
	wavGroup_->put(wavFileBtn_, 0, 2); //0.8, 0, 0.2, 0.25, 2, 2, 2, 2);
	wavGroup_->put(wavStartLabel_, 1, 0); //0, 0.25, 0.4, 0.25, 2, 2, 2, 2);
	wavGroup_->put(wavStartEditor_, 1, 1, 1, 2); //0.4, 0.25, 0.6, 0.25, 2, 2, 2, 2);
	wavGroup_->put(wavAmntTypeSelector_, 2, 0, 1, 3); //0, 0.5, 1, 0.25, 2, 2, 2, 2);
	wavGroup_->put(wavAmntEditor_, 3, 0, 1, 3); //0, 0.75, 1, 0.25, 2, 2, 2, 2);

	morphStartLabel_->setText("START");
	morphStartEditor_->setMin(0, true);
	morphAmntLabel_->setText("AMOUNT");
	morphAmntEditor_->setMin(1, true);
	morphAmntEditor_->setMax(1, false);
	morphAmntEditor_->setValue(1);

	tgl = new CR42YToggle(ui);
	tgl->setText("XFADE");
	morphTypeSelector_->putToggle(tgl, 0, 0, 0.5, 0.5, 0, 0, 0, 0);
	tgl = new CR42YToggle(ui);
	tgl->setText("SPECTRAL");
	morphTypeSelector_->putToggle(tgl, 0.5, 0, 0.5, 0.5, 0, 0, 0, 0);
	tgl = new CR42YToggle(ui);
	tgl->setText("SPECTRAL (0 FUND)");
	morphTypeSelector_->putToggle(tgl, 0, 0.5, 0.5, 0.5, 0, 0, 0, 0);
	tgl = new CR42YToggle(ui);
	tgl->setText("SPECTRAL (0 ALL)");
	morphTypeSelector_->putToggle(tgl, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0);

	morphTypeSelector_->select(0);

	morphGroup_->configureColumn(0, 2, 0, 0.003, 0, 0);
	morphGroup_->configureColumn(1, 3, 0.003, 0, 0, 0);

	morphGroup_->configureRow(0, 1, 0, 0.003, 0, 0);
	morphGroup_->configureRow(1, 1, 0.003, 0.003, 0, 0);
	morphGroup_->configureRow(2, 2, 0.003, 0, 0, 0);

	morphGroup_->put(morphStartLabel_, 0, 0); //0, 0, 0.4, 0.2, 2, 2, 2, 2);
	morphGroup_->put(morphStartEditor_, 0, 1); //0.4, 0, 0.6, 0.2, 2, 2, 2, 2);
	morphGroup_->put(morphAmntLabel_, 1, 0); //0, 0.2, 0.4, 0.2, 2, 2, 2, 2);
	morphGroup_->put(morphAmntEditor_, 1, 1); //0.4, 0.2, 0.6, 0.2, 2, 2, 2, 2);
	morphGroup_->put(morphTypeSelector_, 2, 0, 1, 2); //0, 0.4, 1, 0.6, 2, 2, 2, 2);

	put(opSelector_, 0, 0); //0, 0, 1, 0.2, 2, 2, 2, 2);
	put(mathGroup_, 1, 0); //0, 0.2, 1, 0.65, 2, 2, 2, 2);
	put(wavGroup_, 1, 0); //0, 0.2, 1, 0.65, 2, 2, 2, 2);
	put(morphGroup_, 1, 0); //0, 0.2, 1, 0.65, 2, 2, 2, 2);
	put(addBtn_, 2, 0); //0.7, 0.85, 0.3, 0.15, 2, 2, 2, 2);
}

CR42YWFBulkEditor::~CR42YWFBulkEditor()
{
	delete opSelector_;
	delete addBtn_;

	delete mathGroup_;
	delete mathFuncLabel_;
	delete mathFuncEntry_;
	delete mathStartLabel_;
	delete mathStartEditor_;
	delete mathAmntLabel_;
	delete mathAmntEditor_;

	delete wavGroup_;
	delete wavFileEntry_;
	delete wavFileBtn_;
	delete wavStartLabel_;
	delete wavStartEditor_;
	delete wavAmntTypeSelector_;
	delete wavAmntEditor_;

	delete morphGroup_;
	delete morphStartLabel_;
	delete morphStartEditor_;
	delete morphAmntLabel_;
	delete morphAmntEditor_;
	delete morphTypeSelector_;
}

void CR42YWFBulkEditor::setController(WavetableEditController* controller)
{
	controller_ = controller;
}

void CR42YWFBulkEditor::update()
{
	if (controller_)
	{
		mathStartEditor_->setMax(controller_->getWavetableHeight(), true);
		wavStartEditor_->setMax(controller_->getWavetableHeight(), true);
		morphStartEditor_->setMax(controller_->getWavetableHeight() - 1, true);

		mathStartEditor_->setValue(controller_->selectedWaveform() + 1);
		wavStartEditor_->setValue(controller_->selectedWaveform() + 1);
		morphStartEditor_->setValue(controller_->selectedWaveform() + 1);
	}
}

void CR42YWFBulkEditor::on_realize()
{
	CR42YGrid::on_realize();
	opSelectCallback(0);
}

void CR42YWFBulkEditor::on_show()
{
	CR42YGrid::on_show();
	opSelectCallback(opSelector_->selected());
}

void CR42YWFBulkEditor::opSelectCallback(int selected)
{
	mathGroup_->hide();
	wavGroup_->hide();
	morphGroup_->hide();

	switch (selected)
	{
	case 0:
		mathGroup_->show();
		break;
	case 1:
		wavGroup_->show();
		break;
	case 2:
		morphGroup_->show();
		break;
	default:
		break;
	}
}

void CR42YWFBulkEditor::addCallback()
{
	if (controller_)
	{
		switch (opSelector_->selected())
		{
		case 0:
			controller_->addFunctionWaveforms(mathStartEditor_->value(),
					mathAmntEditor_->value(), mathFuncEntry_->get_text());
			break;
		case 1:
			if (wavAmntTypeSelector_->selected() == 0)
			{
				controller_->addWavWaveforms(wavStartEditor_->value(),
						wavAmntEditor_->value(), -1, wavFileEntry_->get_text());
			}
			else
			{
				controller_->addWavWaveforms(wavStartEditor_->value(), -1,
						wavAmntEditor_->value(), wavFileEntry_->get_text());
			}
			break;
		case 2:
		{
			switch (morphTypeSelector_->selected())
			{
			case 0:
				controller_->crossfadeWaveforms(morphStartEditor_->value(),
						morphAmntEditor_->value());
				break;
			case 1:
				controller_->spectralFadeWaveforms(morphStartEditor_->value(),
						morphAmntEditor_->value(), false, false);
				break;
			case 2:
				controller_->spectralFadeWaveforms(morphStartEditor_->value(),
						morphAmntEditor_->value(), false, true);
				break;
			case 3:
				controller_->spectralFadeWaveforms(morphStartEditor_->value(),
						morphAmntEditor_->value(), true, true);
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}
	}
}

void CR42YWFBulkEditor::fileChooseCallback()
{
	Gtk::FileChooserDialog chooser("Choose file",
			Gtk::FILE_CHOOSER_ACTION_OPEN);
	chooser.set_transient_for(*(Gtk::Window*) get_toplevel());
	chooser.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	chooser.add_button("Open", Gtk::RESPONSE_OK);

	int result = chooser.run();

	if (result == Gtk::RESPONSE_OK)
	{
		wavFileEntry_->set_text(chooser.get_filename());
	}
}

} /* namespace cr42y */

