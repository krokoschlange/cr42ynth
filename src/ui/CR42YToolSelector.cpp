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
		CR42YRelativeContainer(ui),
		controller_(controller)
{
	CR42YToggle* tgl = new CR42YToggle(ui);
	Cairo::RefPtr<Cairo::ImageSurface> surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/linCurve.png");
	tgl->setText("LINEAR");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::TRI_SLOPE));
	put(tgl, 0, 0, 0.333, 0.5, 2, 2, 2, 2);

	selectCallback(tgl, WavetableEditController::TRI_SLOPE);

	tgl = new CR42YToggle(ui);
	surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/freeDraw.png");
	tgl->setText("FREE");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::FREE));
	put(tgl, 0.333, 0, 0.333, 0.5, 2, 2, 2, 2);

	tgl = new CR42YToggle(ui);
	surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/sinCurve.png");
	tgl->setText("SIN");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::SIN_SLOPE));
	put(tgl, 0.666, 0, 0.333, 0.5, 2, 2, 2, 2);

	tgl = new CR42YToggle(ui);
	surf = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/sinHalfCurve.png");
	tgl->setText("SIN HALF");
	tgl->setSurfActive(surf);
	tgl->setSurfInactive(surf);
	tgl->signalClicked().connect(sigc::bind<CR42YToggle*, int>(sigc::mem_fun(this, &CR42YToolSelector::selectCallback), tgl, WavetableEditController::SIN_HALF));
	put(tgl, 0, 0.5, 0.333, 0.5, 2, 2, 2, 2);
}

CR42YToolSelector::~CR42YToolSelector()
{
	std::vector<Gtk::Widget*> children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void CR42YToolSelector::selectCallback(CR42YToggle* source, int tool)
{
	controller_->setTool((WavetableEditController::TOOL) tool);

	std::vector<Gtk::Widget*> children = get_children();
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == source)
		{
			children[i]->set_state(Gtk::STATE_ACTIVE);
		}
		else
		{
			children[i]->set_state(Gtk::STATE_NORMAL);
		}
	}
}

} /* namespace cr42y */
