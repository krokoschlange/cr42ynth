#include "CR42YOSCPanelScroll.h"

#include "common.h"
#include "CR42YnthCommunicator.h"

#include "CR42YButton.h"
#include "CR42YOSCPanel.h"
#include "CR42YUI.h"
#include "OSCSettingsController.h"

namespace cr42y
{

CR42YOSCPanelScroll::CR42YOSCPanelScroll(CR42YUI* ui, CR42YnthCommunicator* comm, WavetableEditController* wtEditController, CR42YToggleSelector* viewSelector) :
		Glib::ObjectBase("CR42YOSCPanelScroll"),
		CR42YGrid(ui),
		leftBtn_(new CR42YButton(ui)),
		rightBtn_(new CR42YButton(ui)),
		scrollIndex_(0),
		controller_(new OSCSettingsController(comm, wtEditController))
{
	configureColumn(0, 1, 0, 0, 70, 0);
	for (int i = 0; i < panelAmount_; i++)
	{
		configureColumn(i + 1, 4, 0, 0, 70, 0);
	}
	configureColumn(panelAmount_ + 1, 1, 0, 0, 70, 0);

	configureRow(0, 1, 0, 0, 0, 0);

	leftBtn_->setSurfActive(
			Cairo::ImageSurface::create_from_png(
					ui->resourceRoot() + "media/left.png"));
	leftBtn_->setSurfInactive(
			Cairo::ImageSurface::create_from_png(
					ui->resourceRoot() + "media/left.png"));

	leftBtn_->signalClicked().connect(
			sigc::mem_fun(this, &CR42YOSCPanelScroll::leftCallback));

	put(leftBtn_, 0, 0);

	rightBtn_->setSurfActive(
			Cairo::ImageSurface::create_from_png(
					ui->resourceRoot() + "media/right.png"));
	rightBtn_->setSurfInactive(
			Cairo::ImageSurface::create_from_png(
					ui->resourceRoot() + "media/right.png"));

	rightBtn_->signalClicked().connect(
			sigc::mem_fun(this, &CR42YOSCPanelScroll::rightCallback));

	put(rightBtn_, 0, panelAmount_ + 1);

	for (int i = 0; i < panelAmount_; i++)
	{
		CR42YOSCPanel* panel = new CR42YOSCPanel(ui, wtEditController, viewSelector);
		put(panel, 0, i + 1);
		panels_.push_back(panel);
	}
}

CR42YOSCPanelScroll::~CR42YOSCPanelScroll()
{
	for (int i = 0; i < panels_.size(); i++)
	{
		remove(*(panels_[i]));
		delete panels_[i];
	}
}

void CR42YOSCPanelScroll::update()
{
	for (int i = 0; i < panels_.size(); i++)
	{
		panels_[i]->connectData(i + scrollIndex_, controller_);
	}
	if (scrollIndex_ > 0)
	{
		leftBtn_->set_sensitive(true);
	}
	else
	{
		leftBtn_->set_sensitive(false);
	}
	if (scrollIndex_ < CR42Ynth_OSC_COUNT - panelAmount_)
	{
		rightBtn_->set_sensitive(true);
	}
	else
	{
		rightBtn_->set_sensitive(false);
	}
}

void CR42YOSCPanelScroll::leftCallback()
{
	scrollIndex_--;
	scrollIndex_ = scrollIndex_ < 0 ? 0 : scrollIndex_;
	update();
}

void CR42YOSCPanelScroll::rightCallback()
{
	scrollIndex_++;
	scrollIndex_ =
			scrollIndex_ >= CR42Ynth_OSC_COUNT ? CR42Ynth_OSC_COUNT - 1 :
													scrollIndex_;
	update();
}

} /* namespace cr42y */
