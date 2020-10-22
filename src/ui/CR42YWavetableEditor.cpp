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

#include "CR42YWavetableEditor.h"
#include "CR42YWaveformEditor.h"
#include "CR42YHarmonicsView.h"
#include "CR42YWavetableView.h"
#include "WavetableEditController.h"
#include "WavetableEditData.h"
#include "CR42YButton.h"
#include "CR42YIntegerEditor.h"
#include "CR42YVScrollbar.h"
#include "CR42YWaveformEditorControlPanel.h"
#include "CR42YHarmonicsEditor.h"
#include "CR42YHScrollbar.h"
#include "CR42YToolSelector.h"
#include "CR42YWFPartEditor.h"
#include "CR42YWFBulkEditor.h"

namespace cr42y
{

CR42YWavetableEditor::CR42YWavetableEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWavetableEditor"),
		CR42YGrid(ui),
		wfEditor_(new CR42YWaveformEditor(ui)),
		controller_(new WavetableEditController()),
		wfeControllerPanel_(
				new CR42YWaveformEditorControlPanel(ui, wfEditor_,
						controller_)),
		harmonicsView_(new CR42YHarmonicsView(ui)),
		wtView_(new CR42YWavetableView(ui)),
		addWaveformButton_(new CR42YButton(ui)),
		harmonicsEditor_(new CR42YHarmonicsEditor(ui)),
		toolSelector_(new CR42YToolSelector(ui, controller_)),
		wfPartEditor_(new CR42YWFPartEditor(ui)),
		wfBulkEditor_(new CR42YWFBulkEditor(ui))
{
	configureColumn(0, 1, 0.003, 0.003, 0, 0);
	configureColumn(1, 7, 0.003, 0.003, 0, 0);
	configureColumn(2, 1, 0.003, 0.003, 0, 0);
	configureColumn(3, 1, 0, 1, 0, 15);

	configureRow(0, 3, 0.003, 0.003, 0, 0);
	configureRow(1, 1, 0.003, 0.003, 0, 0);
	configureRow(2, 5, 0.003, 0.003, 0, 0);
	configureRow(3, 5, 0.003, 0.003, 0, 0);
	configureRow(4, 1, 0.003, 0.003, 0, 0);
	configureRow(5, 3, 0.003, 0.003, 0, 0);
	configureRow(6, 2, 0.003, 0.003, 0, 0);
	configureRow(7, 1, 0, 1, 0, 15);

	wfEditor_->setController(controller_);
	harmonicsView_->setController(controller_);
	wfPartEditor_->setController(controller_);
	wfBulkEditor_->setController(controller_);

	addWaveformButton_->setText("Add WF");
	addWaveformButton_->signalClicked().connect(
			sigc::mem_fun(this, &CR42YWavetableEditor::waveformAddCallback));

	wtView_->setController(controller_);
	CR42YVScrollbar* wtViewBar = new CR42YVScrollbar(ui);
	Gtk::Adjustment wtViewHadj(0, 0, 0, 0, 0, 0);
	Gtk::Viewport* wtViewVP = new Gtk::Viewport(wtViewHadj,
			*wtViewBar->get_adjustment());
	wtViewVP->signal_scroll_event().connect(
			sigc::mem_fun(wtViewBar, &CR42YVScrollbar::scroll_event));
	wtViewVP->set_shadow_type(Gtk::SHADOW_NONE);
	wtViewVP->set_size_request(10, 10);
	wtViewVP->add(*wtView_);

	harmonicsEditor_->setController(controller_);
	CR42YHScrollbar* harmEdBar = new CR42YHScrollbar(ui);
	Gtk::Adjustment harmEdVadj(0, 0, 0, 0, 0, 0);
	Gtk::Viewport* harmEdVP = new Gtk::Viewport(*harmEdBar->get_adjustment(),
			harmEdVadj);
	harmEdVP->signal_scroll_event().connect(
			sigc::mem_fun(harmEdBar, &CR42YHScrollbar::scroll_event));
	harmEdVP->set_shadow_type(Gtk::SHADOW_NONE);
	harmEdVP->set_size_request(10, 10);
	harmEdVP->add(*harmonicsEditor_);

	/*put(toolSelector_, 0, 0, 0.1, 0.15, 0.003, 0.003, 0.003, 0.003);
	 put(harmonicsView_, 0.1, 0, 0.8, 0.2, 0.003, 0.003, 0.003, 0.003);
	 put(wtViewBar, 0.988, 0, 0.012, 0.9, 0.003, 0.003, 0.003, 0.003);
	 put(wtViewVP, 0.9, 0, 0.088, 0.9, 0.003, 0.003, 0.003, 0.003);
	 put(wfPartEditor_, 0, 0.2, 0.1, 0.3, 0.003, 0.003, 0.003, 0.003);
	 put(wfEditor_, 0.1, 0.2, 0.8, 0.5, 0.003, 0.003, 0.003, 0.003);
	 put(wfeControllerPanel_, 0.1, 0.7, 0.8, 35, 0.003, 0.003, 0.003, 0.003);
	 put(addWaveformButton_, 0.9, 0.9, 0.1, 0.1, 0.003, 0.003, 0.003, 0.003);
	 put(harmEdVP, 0.1, 0.7, 0.8, 0.27, 0.003, 0.003, 0.003, 0.003);
	 put(harmEdBar, 0.1, 0.97, 0.8, 0.03, 0.003, 0.003, 0.003, 0.003);
	 put(wfBulkEditor_, 0, 0.5, 0.1, 0.3, 0.003, 0.003, 0.003, 0.003);*/
	//put(harmonicsEditor_,		0.1,		 0.7,		0.8,		 0.3,		  5,		 40,		 0,		 5);
	put(toolSelector_, 0, 0);
	put(harmonicsView_, 0, 1, 2);
	put(wtViewBar, 0, 3, 6);
	put(wtViewVP, 0, 2, 6);
	put(wfPartEditor_, 1, 0, 2);
	put(wfEditor_, 2, 1, 2);
	put(wfeControllerPanel_, 4, 1);
	put(addWaveformButton_, 6, 2, 2, 2);
	put(harmEdVP, 5, 1, 2);
	put(harmEdBar, 7, 1);
	put(wfBulkEditor_, 3, 0, 2);

	//wfEditor_->waveformUpdateSignal().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformUpdateCallback));
	//wtView_->waveformRemovedSignal().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformUpdateCallback));

	controller_->signalSelectedChanged().connect(
			sigc::mem_fun(this,
					&CR42YWavetableEditor::selectionChangedCallback));
	controller_->signalSelectedChangedDone().connect(
			sigc::mem_fun(this,
					&CR42YWavetableEditor::selectionChangedDoneCallback));
}

CR42YWavetableEditor::~CR42YWavetableEditor()
{
	std::vector<Gtk::Widget*> children = get_children();

	for (int i = 0; i < children.size(); i++)
	{
		remove(*(children[i]));
		delete children[i];
	}

	delete controller_;
}

void CR42YWavetableEditor::setData(WavetableEditData* data)
{
	controller_->setData(data, true);
}

WavetableEditController* CR42YWavetableEditor::getController()
{
	return controller_;
}

void CR42YWavetableEditor::on_realize()
{
	CR42YGrid::on_realize();
	selectionChangedDoneCallback();
}

void CR42YWavetableEditor::waveformAddCallback()
{
	controller_->addWaveform(-1);
}

void CR42YWavetableEditor::selectionChangedCallback()
{
	wfEditor_->updateButtons();
	wfEditor_->queue_draw();
	harmonicsView_->queue_draw();
}

void CR42YWavetableEditor::selectionChangedDoneCallback()
{
	wfEditor_->updateButtons();
	wfEditor_->queue_draw();
	harmonicsView_->queue_draw();
	wtView_->update();
	wtView_->queue_draw();

	harmonicsEditor_->update();
	harmonicsEditor_->queue_draw();

	wfPartEditor_->update();

	wfBulkEditor_->update();

	toolSelector_->update();
}

} /* namespace cr42y */
