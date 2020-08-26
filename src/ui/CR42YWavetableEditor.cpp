/*
 * CR42YWavetableEditor.cpp
 *
 *  Created on: 05.08.2020
 *      Author: fabian
 */

#include "CR42YWavetableEditor.h"
#include "CR42YWaveformEditor.h"
#include "CR42YHarmonicsView.h"
#include "CR42YWavetableView.h"
#include "WavetableEditController.h"
#include "WavetableEditData.h"
#include "CR42YButton.h"
#include "CR42YEntry.h"
#include "CR42YIntegerEntry.h"

namespace cr42y
{

CR42YWavetableEditor::CR42YWavetableEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWavetableEditor"),
		CR42YRelativeContainer(ui),
		wfEditor_(new CR42YWaveformEditor(ui)),
		controller_(new WavetableEditController()),
		harmonicsView_(new CR42YHarmonicsView(ui)),
		wtView_(new CR42YWavetableView(ui)),
		addWaveformButton_(new CR42YButton(ui))
{
	WavetableEditData* data = new WavetableEditData(4096);
	controller_->setData(data);
	wfEditor_->setController(controller_);
	harmonicsView_->setController(controller_);
	wtView_->setController(controller_);
	addWaveformButton_->setText("Add WF");
	addWaveformButton_->signalClicked().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformAddCallback));

	//Gtk::Adjustment vadj(0, 0, 100, 1, 10, 10);
	Gtk::VScrollbar* bar = new Gtk::VScrollbar();


	Gtk::Adjustment hadj(0, 0, 0, 0, 0, 0);
	Gtk::Viewport* vp = new Gtk::Viewport(hadj, *bar->get_adjustment());
	vp->set_shadow_type(Gtk::SHADOW_NONE);
	vp->add(*wtView_);
	//btn->set_size_request(100, 1000);

	CR42YEntry* entry = new CR42YEntry(ui);
	entry->set_size_request(50, 100);
	
	CR42YIntegerEntry* ientry = new CR42YIntegerEntry(ui);
	
	put(ientry, 0, 0.2, 0.1, 0.2, 5, 5);
	put(entry, 0, 0, 0.1, 0.2, 10, 10);
	put(wfEditor_, 0.1, 0.2, 0.8, 0.8, 5, 2, 5, 5);
	put(harmonicsView_, 0.1, 0, 0.8, 0.2, 5, 5, 5, 3);
	put(bar, 1, 0, 15, 0.9, -30);
	put(vp, 0.9, 0, 0.1, 0.9, 0, 5, 20, 5);
	put(addWaveformButton_, 0.9, 0.9, 0.1, 0.1, 5, 5, 5, 5);

	wfEditor_->waveformUpdateSignal().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformUpdateCallback));
	wtView_->waveformRemovedSignal().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformUpdateCallback));


}

CR42YWavetableEditor::~CR42YWavetableEditor()
{
}

void CR42YWavetableEditor::waveformUpdateCallback()
{
	wfEditor_->queue_draw();
	wfEditor_->updateButtons();
	harmonicsView_->queue_draw();
	wtView_->queue_draw();
}

void CR42YWavetableEditor::waveformAddCallback()
{
	controller_->addWaveform(-1);
	wtView_->queue_draw();
	wtView_->update();
}

} /* namespace cr42y */
