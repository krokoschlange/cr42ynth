#include "CR42YOSCPanel.h"

#include "CR42YDial.h"
#include "CR42YToggle.h"
#include "CR42YUI.h"
#include "CR42YWFView.h"
#include "OSCSettingsController.h"

namespace cr42y
{

CR42YOSCPanel::CR42YOSCPanel(CR42YUI* ui) :
		Glib::ObjectBase("CR42YOSCPanel"),
		CR42YGrid(ui),
		wfView_(new CR42YWFView(ui)),
		oscToggle_(new CR42YToggle(ui)),
		volumeDial_(new CR42YDial(ui))
{
	configureRow(0, 1, 0.003, 0.003, 0, 0);
	configureRow(1, 5, 0.003, 0.003, 0, 0);
	configureRow(2, 2, 0.003, 0.003, 0, 0);

	configureColumn(0, 1, 0.003, 0.003, 0, 0);
	configureColumn(1, 2, 0.003, 0.003, 0, 0);
	configureColumn(2, 5, 0.003, 0.003, 0, 0);

	oscToggle_->setForcedSizeRatio(1);
	oscToggle_->setSurfActive(Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/power.png"));
	oscToggle_->setSurfInactive(Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/power_off.png"));

	put(oscToggle_, 0, 0);
	put(wfView_, 1, 0, 1, 3);
	put(volumeDial_, 2, 0, 1, 2);
}

CR42YOSCPanel::~CR42YOSCPanel()
{
	delete wfView_;
	delete oscToggle_;
	delete volumeDial_;
}

void CR42YOSCPanel::connectData(int oscIndex, OSCSettingsController* controller_)
{
	wfView_->setData(controller_->getEditData(oscIndex));
	//TODO: others
}

} /* namespace cr42y */
