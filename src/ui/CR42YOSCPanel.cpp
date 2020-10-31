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

#include "CR42YControlDial.h"
#include "CR42YControlToggle.h"
#include "CR42YUI.h"
#include "CR42YWFView.h"
#include "OSCSettingsController.h"
#include "OscillatorControls.h"

namespace cr42y
{

CR42YOSCPanel::CR42YOSCPanel(CR42YUI* ui, WavetableEditController* wtEditController, CR42YToggleSelector* viewSelector) :
		Glib::ObjectBase("CR42YOSCPanel"),
		CR42YGrid(ui),
		wfView_(new CR42YWFView(ui, wtEditController, viewSelector)),
		idxLabel_(new CR42YLabel(ui)),
		oscToggle_(new CR42YControlToggle(ui)),
		volumeDial_(new CR42YControlDial(ui))
{
	configureRow(0, 1, 0.003, 0.003, 0, 0);
	configureRow(1, 5, 0.003, 0.003, 0, 0);
	configureRow(2, 2, 0.003, 0.003, 0, 0);

	configureColumn(0, 1, 0.003, 0.003, 0, 0);
	configureColumn(1, 1, 0.003, 0.003, 0, 0);
	configureColumn(2, 5, 0.003, 0.003, 0, 0);

	oscToggle_->setForcedSizeRatio(1);
	oscToggle_->setSurfActive(Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/power.png"));
	oscToggle_->setSurfInactive(Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/power_off.png"));

	idxLabel_->setForcedSizeRatio(1);

	put(oscToggle_, 0, 0);
	put(idxLabel_, 0, 1);
	put(wfView_, 1, 0, 1, 3);
	put(volumeDial_, 2, 0, 1, 2);
}

CR42YOSCPanel::~CR42YOSCPanel()
{
	delete wfView_;
	delete idxLabel_;
	delete oscToggle_;
	delete volumeDial_;
}

void CR42YOSCPanel::connectData(int oscIndex, OSCSettingsController* controller_)
{
	wfView_->setData(controller_->getEditData(oscIndex));
	//TODO: others

	idxLabel_->setText(std::to_string(oscIndex + 1));
	idxLabel_->queue_draw();

	volumeDial_->connectControl(controller_->getControls(oscIndex)->getVolumeCtrl());

	oscToggle_->connectControl(controller_->getControls(oscIndex)->getActiveCtrl());
}

} /* namespace cr42y */
