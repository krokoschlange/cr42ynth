#include "CR42YOSCPanel.h"

#include "CR42YControlDial.h"
#include "CR42YToggle.h"
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
		oscToggle_(new CR42YToggle(ui)),
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
}

} /* namespace cr42y */
