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


#include "CR42YToolSelector.h"
#include "CR42YToggle.h"
#include "CR42YButton.h"
#include "CR42YUI.h"
#include "WavetableEditController.h"

namespace cr42y
{

CR42YToolSelector::CR42YToolSelector(CR42YUI* ui,
		WavetableEditController* controller) :
		Glib::ObjectBase("CR42YToolSelector"),
		CR42YToggleSelector(ui),
		controller_(controller),
		undoBtn_(new CR42YButton(ui)),
		redoBtn_(new CR42YButton(ui))
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

	undoBtn_->setText("UNDO");
	redoBtn_->setText("REDO");

	put(undoBtn_, 0.666, 0.5, 0.333, 0.25, 0.003, 0.003, 0.003, 0.003);
	put(redoBtn_, 0.666, 0.75, 0.333, 0.25, 0.003, 0.003, 0.003, 0.003);

	undoBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YToolSelector::undoCallback));
	redoBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YToolSelector::redoCallback));
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

void CR42YToolSelector::update()
{
	if (controller_)
	{
		undoBtn_->set_sensitive(controller_->undoPossible());
		redoBtn_->set_sensitive(controller_->redoPossible());
	}
}

void CR42YToolSelector::undoCallback()
{
	if (controller_)
	{
		controller_->undo();
	}
}

void CR42YToolSelector::redoCallback()
{
	if (controller_)
	{
		controller_->redo();
	}
}

} /* namespace cr42y */
