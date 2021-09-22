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

#include "CR42YAutomationSettings.h"

#include "AutomationEditController.h"
#include "CR42YLabel.h"
#include "CR42YIntegerEditor.h"
#include "CR42YControlIntegerEditor.h"
#include "CR42YControlToggleSelector.h"
#include "CR42YControlDial.h"
#include "CR42YToggle.h"

namespace cr42y
{

CR42YAutomationSettings::CR42YAutomationSettings(CR42YUI* ui, AutomationEditController* controller) :
		CR42YGrid(ui),
		controller_(controller),
		gridXLabel_(new CR42YLabel(ui)),
		gridYLabel_(new CR42YLabel(ui)),
		gridXEditor_(new CR42YIntegerEditor(ui)),
		gridYEditor_(new CR42YIntegerEditor(ui)),
		settingsGrid_(new CR42YGrid(ui)),
		typeSelector_(new CR42YControlToggleSelector(ui)),
		sustainLabel_(new CR42YLabel(ui)),
		sustainEditor_(new CR42YControlIntegerEditor(ui)),
		syncSelector_(new CR42YControlToggleSelector(ui)),
		timingSelector_(new CR42YControlToggleSelector(ui)),
		secondsTimingGrid_(new CR42YGrid(ui)),
		secondsLabel_(new CR42YLabel(ui)),
		secondsDial_(new CR42YControlDial(ui)),
		beatsTimingGrid_(new CR42YGrid(ui)),
		beatsLabel_(new CR42YLabel(ui)),
		numeratorEditor_(new CR42YControlIntegerEditor(ui)),
		lineLabel_(new CR42YLabel(ui)),
		denominatorEditor_(new CR42YControlIntegerEditor(ui))
{
	configureColumn(0, 2, 0, 0, 0, 0);
	configureColumn(1, 1, 0, 0, 0, 0);
	configureColumn(2, 1, 0, 0, 0, 0);
	configureColumn(3, 2, 0, 0, 0, 0);
	
	configureRow(0, 2, 0, 0, 0, 0);
	configureRow(1, 2, 0, 0, 0, 0);
	configureRow(2, 3, 0, 0, 0, 0);
	configureRow(3, 8, 0, 0, 0, 0);
	configureRow(4, 2, 0, 0, 0, 0);
	configureRow(5, 2, 0, 0, 0, 0);
	
	
	gridXLabel_->setText("Grid X");
	gridYLabel_->setText("Grid Y");
	
	gridXEditor_->setMin(0, true);
	gridXEditor_->setMax(0, false);
	
	gridYEditor_->setMin(0, true);
	gridYEditor_->setMax(0, false);
	
	CR42YToggle* tgl = new CR42YToggle(ui);
	tgl->setText("ENV");
	typeSelector_->putToggle(tgl, 0, 0, 0.5, 1);
	tgl = new CR42YToggle(ui);
	tgl->setText("LFO");
	typeSelector_->putToggle(tgl, 0.5, 0, 0.5, 1);
	
	typeSelector_->signalSelected().connect(sigc::mem_fun(this, &CR42YAutomationSettings::typeSelectCallback));
	
	tgl = new CR42YToggle(ui);
	tgl->setText("NOTE");
	syncSelector_->putToggle(tgl, 0, 0, 1, 0.333);
	tgl = new CR42YToggle(ui);
	tgl->setText("SYNC");
	syncSelector_->putToggle(tgl, 0, 0.333, 1, 0.333);
	tgl = new CR42YToggle(ui);
	tgl->setText("FREE");
	syncSelector_->putToggle(tgl, 0, 0.666, 1, 0.333);
	
	tgl = new CR42YToggle(ui);
	tgl->setText("SEC");
	timingSelector_->putToggle(tgl, 0, 0.5, 1, 0.5);
	tgl = new CR42YToggle(ui);
	tgl->setText("BEATS");
	timingSelector_->putToggle(tgl, 0, 0, 1, 0.5);
	
	timingSelector_->signalSelected().connect(sigc::mem_fun(this, &CR42YAutomationSettings::timingSelectCallback));
	
	secondsTimingGrid_->configureRow(0, 2, 0, 0, 0, 0);
	secondsTimingGrid_->configureRow(1, 3, 0, 0, 0, 0);
	secondsTimingGrid_->configureRow(2, 1, 0, 0, 0, 0);
	secondsTimingGrid_->configureRow(3, 2, 3, 3, 0, 8);
	
	secondsLabel_->setText("SEC");
	
	secondsTimingGrid_->put(secondsLabel_, 0, 0, 1, 1);
	secondsTimingGrid_->put(secondsDial_, 1, 0, 1, 1);
	
	beatsTimingGrid_->configureRow(0, 1, 0, 0, 0, 0);
	beatsTimingGrid_->configureRow(1, 1, 0, 0, 0, 0);
	beatsTimingGrid_->configureRow(2, 1, 3, 3, 0, 8);
	beatsTimingGrid_->configureRow(3, 1, 0, 0, 0, 0);
	//beatsTimingGrid_->configureRow(4, 1, 0, 0, 0, 0);
	
	beatsLabel_->setText("BEATS");
	numeratorEditor_->setMin(1, true);
	numeratorEditor_->setMax(0, false);
	denominatorEditor_->setMin(1, true);
	denominatorEditor_->setMax(0, false);
	
	beatsTimingGrid_->put(beatsLabel_, 0, 0, 1, 1);
	beatsTimingGrid_->put(numeratorEditor_, 1, 0, 1, 1);
	beatsTimingGrid_->put(lineLabel_, 2, 0, 1, 1);
	beatsTimingGrid_->put(denominatorEditor_, 3, 0, 1, 1);
	
	sustainLabel_->setText("Sustain");
	
	sustainEditor_->signalChanged().connect(sigc::hide(sigc::mem_fun(controller_->dataChangedSignal(), &sigc::signal<void>::emit)));
	
	put(gridXLabel_, 0, 0, 1, 2);
	put(gridYLabel_, 0, 2, 1, 2);
	put(gridXEditor_, 1, 0, 1, 2);
	put(gridYEditor_, 1, 2, 1, 2);
	put(typeSelector_, 2, 0, 1, 4);
	put(syncSelector_, 3, 0, 3, 1); 
	put(timingSelector_, 3, 3, 1, 1);
	put(secondsTimingGrid_, 3, 1, 1, 2);
	put(beatsTimingGrid_, 3, 1, 1, 2);
	put(sustainLabel_, 4, 1, 1, 2);
	put(sustainEditor_, 5, 1, 1, 2);
	
	controller_->dataChangedSignal().connect(sigc::mem_fun(this, &CR42YAutomationSettings::dataChangeCallback));
}

CR42YAutomationSettings::~CR42YAutomationSettings()
{
	delete gridXLabel_;
	delete gridYLabel_;
	delete gridXEditor_;
	delete gridYEditor_;
	delete typeSelector_;
	delete syncSelector_;
	delete timingSelector_;
	delete secondsLabel_;
	delete secondsDial_;
	delete secondsTimingGrid_;
	delete beatsLabel_;
	delete numeratorEditor_;
	delete lineLabel_;
	delete denominatorEditor_;
	delete beatsTimingGrid_;
	delete settingsGrid_;
}

void CR42YAutomationSettings::on_realize()
{
	CR42YGrid::on_realize();
	timingSelectCallback(timingSelector_->selected());
}

void CR42YAutomationSettings::on_show()
{
	CR42YGrid::on_show();
	timingSelectCallback(timingSelector_->selected());
	typeSelectCallback(typeSelector_->selected());
}

void CR42YAutomationSettings::update()
{
	typeSelector_->connectControl(controller_->typeControl());
	secondsDial_->connectControl(controller_->lengthSecondsControl());
	syncSelector_->connectControl(controller_->syncControl());
	timingSelector_->connectControl(controller_->useBeatsLengthControl());
	secondsDial_->connectControl(controller_->lengthSecondsControl());
	numeratorEditor_->connectControl(controller_->lengthBeatsNumeratorControl());
	denominatorEditor_->connectControl(controller_->lengthBeatsDenominatorControl());
	sustainEditor_->connectControl(controller_->sustainControl());
	sustainEditor_->setMax((int) controller_->sectionAmount() - 1);
}

sigc::signal<void, int> cr42y::CR42YAutomationSettings::signalGridX()
{
	return gridXEditor_->signalChanged();
}

sigc::signal<void, int> cr42y::CR42YAutomationSettings::signalGridY()
{
	return gridYEditor_->signalChanged();
}

void CR42YAutomationSettings::timingSelectCallback(int selected)
{
	beatsTimingGrid_->hide();
	secondsTimingGrid_->hide();
	switch (selected)
	{
	case 0:
		secondsTimingGrid_->show_all();
		break;
	case 1:
		beatsTimingGrid_->show_all();
		break;
	default:
		break;
	}
}

void CR42YAutomationSettings::typeSelectCallback(int selected)
{
	if (selected == 0)
	{
		sustainLabel_->show();
		sustainEditor_->show();
	}
	else
	{
		sustainLabel_->hide();
		sustainEditor_->hide();
	}
	controller_->dataChangedSignal().emit();
}

void CR42YAutomationSettings::dataChangeCallback()
{
	sustainEditor_->setMax((int) controller_->sectionAmount() - 1);
	sustainEditor_->setValue(std::min(sustainEditor_->value(), (int) controller_->sectionAmount() - 1));
}

}
