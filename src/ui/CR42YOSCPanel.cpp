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
#include "CR42YOSCPanel.h"

#include "CR42YAutomationDial.h"
#include "CR42YControlDial.h"
#include "CR42YControlToggle.h"
#include "CR42YDetuneEditor.h"
#include "CR42YControlIntegerEditor.h"
#include "CR42YUI.h"
#include "CR42YWFView.h"
#include "OSCSettingsController.h"
#include "OscillatorControls.h"
#include "CR42YTheme.h"

namespace cr42y
{

CR42YOSCPanel::CR42YOSCPanel(CR42YUI* ui, WavetableEditController* wtEditController, CR42YToggleSelector* viewSelector) :
		Glib::ObjectBase("CR42YOSCPanel"),
		CR42YGrid(ui),
		wfView_(new CR42YWFView(ui, wtEditController, viewSelector)),
		idxLabel_(new CR42YLabel(ui)),
		oscToggle_(new CR42YControlToggle(ui)),
		volumeDial_(new CR42YAutomationDial(ui)),
		detuneEditor_(new CR42YDetuneEditor(ui)),
		panDial_(new CR42YAutomationDial(ui)),
		noteShiftDial_(new CR42YAutomationDial(ui)),
		wtPosDial_(new CR42YAutomationDial(ui)),
		unisonAmountEditor_(new CR42YControlIntegerEditor(ui)),
		unisonDetuneDial_(new CR42YAutomationDial(ui)),
		unisonSpreadDial_(new CR42YAutomationDial(ui)),
		phaseShiftDial_(new CR42YAutomationDial(ui)),
		phaseRandDial_(new CR42YControlDial(ui))
{
	configureRow(0, 2, 0.003, 0.003, 0, 0);
	configureRow(1, 10, 0.003, 0.003, 0, 0);
	configureRow(2, 3, 0.003, 0.003, 0, 0);
	configureRow(3, 3, 0.003, 0.003, 0, 0);

	configureColumn(0, 1, 0.003, 0.003, 0, 0);
	configureColumn(1, 1, 0.003, 0.003, 0, 0);
	configureColumn(2, 2, 0.003, 0.003, 0, 0);
	configureColumn(3, 2, 0.003, 0.003, 0, 0);
	configureColumn(4 ,2, 0.003, 0.003, 0, 0);
	configureColumn(5 ,2, 0.003, 0.003, 0, 0);

	oscToggle_->setForcedSizeRatio(1);
	oscToggle_->setSurfActive(Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/power.png"));
	oscToggle_->setSurfInactive(Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/power_off.png"));

	idxLabel_->setForcedSizeRatio(1);
	idxLabel_->setFontSize(CR42YTheme::MIDDLE);
	
	volumeDial_->setText("VOL");
	panDial_->setText("PAN");
	noteShiftDial_->setText("NOTE");
	wtPosDial_->setText("WT POS");
	unisonDetuneDial_->setText("U DET");
	unisonSpreadDial_->setText("U SPREAD");
	phaseShiftDial_->setText("PHASE");
	phaseRandDial_->setText("PHASE RAND");
	
	panDial_->setDefaultValue(0.5);
	panDial_->setValue(0.5);
	
	unisonAmountEditor_->setMin(1);
	unisonAmountEditor_->setMax(16);
	unisonAmountEditor_->setValue(1);
	
	noteShiftDial_->setDefaultValue(0.5);
	noteShiftDial_->setValue(0.5);
	
	wtPosDial_->signalChanged().connect(sigc::mem_fun(this, &CR42YOSCPanel::wtPosCallback));

	put(oscToggle_, 0, 0);
	put(idxLabel_, 0, 1);
	put(wfView_, 1, 0, 1, 6);
	put(volumeDial_, 2, 0, 1, 2);
	put(detuneEditor_, 2, 5, 1, 1);
	put(panDial_, 2, 3, 1, 1);
	put(noteShiftDial_, 3, 5, 1, 1);
	put(wtPosDial_, 2, 2, 1, 1);
	put(unisonAmountEditor_, 3, 0, 1, 2);
	put(unisonDetuneDial_, 3, 2, 1, 1);
	put(unisonSpreadDial_, 3, 3, 1, 1);
	put(phaseShiftDial_, 2, 4, 1, 1);
	put(phaseRandDial_, 3, 4, 1, 1);
}

CR42YOSCPanel::~CR42YOSCPanel()
{
	delete wfView_;
	delete idxLabel_;
	delete oscToggle_;
	delete volumeDial_;
	delete detuneEditor_;
	delete panDial_;
	delete noteShiftDial_;
	delete wtPosDial_;
	delete unisonAmountEditor_;
	delete unisonDetuneDial_;
	delete unisonSpreadDial_;
	delete phaseShiftDial_;
	delete phaseRandDial_;
}

void CR42YOSCPanel::connectData(int oscIndex, OSCSettingsController* controller)
{
	wfView_->setData(controller->getEditData(oscIndex));
	//TODO: others

	idxLabel_->setText(std::to_string(oscIndex + 1));
	idxLabel_->queue_draw();

	volumeDial_->connectControl(controller->getControls(oscIndex)->getVolumeCtrl());
	detuneEditor_->connectControl(controller->getControls(oscIndex)->getDetuneCtrl());
	panDial_->connectControl(controller->getControls(oscIndex)->getPanCtrl());
	noteShiftDial_->connectControl(controller->getControls(oscIndex)->getNoteShiftCtrl());
	wtPosDial_->connectControl(controller->getControls(oscIndex)->getWTPosCtrl());
	unisonAmountEditor_->connectControl(controller->getControls(oscIndex)->getUnisonAmountCtrl());
	unisonDetuneDial_->connectControl(controller->getControls(oscIndex)->getUnisonDetuneCtrl());
	unisonSpreadDial_->connectControl(controller->getControls(oscIndex)->getUnisonSpreadCtrl());
	phaseShiftDial_->connectControl(controller->getControls(oscIndex)->getPhaseShiftCtrl());
	phaseRandDial_->connectControl(controller->getControls(oscIndex)->getPhaseRandCtrl());

	oscToggle_->connectControl(controller->getControls(oscIndex)->getActiveCtrl());
}

void CR42YOSCPanel::wtPosCallback(double value)
{
	wfView_->setSelectedWaveform(value);
}

} /* namespace cr42y */
