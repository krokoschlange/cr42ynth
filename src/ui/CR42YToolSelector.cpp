/*
 * CR42YToolSelector.cpp
 *
 *  Created on: 03.09.2020
 *      Author: fabian
 */

#include "CR42YToolSelector.h"
#include "CR42YToggle.h"
#include "CR42YUI.h"
#include "WavetableEditController.h"

namespace cr42y
{

CR42YToolSelector::CR42YToolSelector(CR42YUI* ui,
		WavetableEditController* controller) :
		Glib::ObjectBase("CR42YToolSelector"),
		CR42YToggleSelector(ui),
		controller_(controller)
{
	CR42YToggle* tgl = new CR42YToggle(ui);
	Cairo::RefPtr<Cairo::ImageSurface> surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/linCurve.png");
	tgl->setText("LINEAR");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	//tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::TRI_SLOPE));
	putToggle(tgl, 0, 0, 0.333, 0.5, 2, 2, 2, 2);
	mapping_.push_back(WavetableEditController::TRI_SLOPE);

	select(0);

	tgl = new CR42YToggle(ui);
	surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/freeDraw.png");
	tgl->setText("FREE");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	//tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::FREE));
	putToggle(tgl, 0.333, 0, 0.333, 0.5, 2, 2, 2, 2);
	mapping_.push_back(WavetableEditController::FREE);

	tgl = new CR42YToggle(ui);
	surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/sinCurve.png");
	tgl->setText("SIN");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	//tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::SIN_SLOPE));
	putToggle(tgl, 0.666, 0, 0.333, 0.5, 2, 2, 2, 2);
	mapping_.push_back(WavetableEditController::SIN_SLOPE);

	tgl = new CR42YToggle(ui);
	surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/sinHalfCurve.png");
	tgl->setText("SIN HALF");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	//tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::SIN_HALF));
	putToggle(tgl, 0, 0.5, 0.333, 0.5, 2, 2, 2, 2);
	mapping_.push_back(WavetableEditController::SIN_HALF);

	signalSelected().connect(sigc::mem_fun(this, &CR42YToolSelector::selectCallback));
}

CR42YToolSelector::~CR42YToolSelector()
{
	std::vector<Gtk::Widget*> children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void CR42YToolSelector::selectCallback(int selected)
{
	if (selected >= 0 && selected < mapping_.size())
	{
		controller_->setTool((WavetableEditController::TOOL) mapping_[selected]);
	}
	else
	{
		controller_->setTool(WavetableEditController::TRI_SLOPE);
	}
}

} /* namespace cr42y */
