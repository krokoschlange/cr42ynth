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
#include "CR42YIntegerEditor.h"
#include "CR42YVScrollbar.h"
#include "CR42YWaveformEditorControlPanel.h"
#include "CR42YHarmonicsEditor.h"
#include "CR42YHScrollbar.h"

namespace cr42y
{

CR42YWavetableEditor::CR42YWavetableEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWavetableEditor"),
		CR42YRelativeContainer(ui),
		wfEditor_(new CR42YWaveformEditor(ui)),
		controller_(new WavetableEditController()),
		wfeControllerPanel_(new CR42YWaveformEditorControlPanel(ui, wfEditor_, controller_)),
		harmonicsView_(new CR42YHarmonicsView(ui)),
		wtView_(new CR42YWavetableView(ui)),
		addWaveformButton_(new CR42YButton(ui)),
		harmonicsEditor_(new CR42YHarmonicsEditor(ui))
{
	setDrawBorder(true);

	WavetableEditData* data = new WavetableEditData(4096);

	controller_->setData(data);
	wfEditor_->setController(controller_);
	harmonicsView_->setController(controller_);

	addWaveformButton_->setText("Add WF");
	addWaveformButton_->signalClicked().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformAddCallback));


	wtView_->setController(controller_);
	CR42YVScrollbar* wtViewBar = new CR42YVScrollbar(ui);
	Gtk::Adjustment wtViewHadj(0, 0, 0, 0, 0, 0);
	Gtk::Viewport* wtViewVP = new Gtk::Viewport(wtViewHadj, *wtViewBar->get_adjustment());
	wtViewVP->signal_scroll_event().connect(sigc::mem_fun(wtViewBar, &CR42YVScrollbar::scroll_event));
	wtViewVP->set_shadow_type(Gtk::SHADOW_NONE);
	wtViewVP->add(*wtView_);


	harmonicsEditor_->setController(controller_);
	CR42YHScrollbar* harmEdBar = new CR42YHScrollbar(ui);
	Gtk::Adjustment harmEdVadj(0, 0, 0, 0, 0, 0);
	Gtk::Viewport* harmEdVP = new Gtk::Viewport(*harmEdBar->get_adjustment(), harmEdVadj);
	harmEdVP->signal_scroll_event().connect(sigc::mem_fun(harmEdBar, &CR42YHScrollbar::scroll_event));
	harmEdVP->set_shadow_type(Gtk::SHADOW_NONE);
	harmEdVP->add(*harmonicsEditor_);

	CR42YEntry* entry = new CR42YEntry(ui);
	entry->set_size_request(50, 100);
	
	CR42YIntegerEditor* ientry = new CR42YIntegerEditor(ui);

	put(entry,					  0,		   0,		0.1,		 0.2,		  5,		  5);
	put(ientry,					  0,		 0.2,		0.1,		 0.2,		  5,		  5);
	put(wfEditor_,				0.1,		 0.2,		0.8,		 0.5,		  5,		  5);
	put(wfeControllerPanel_,	0.1,		 0.7,		0.8,		  35,		  5,		  5,		 0,		 5);
	put(harmonicsView_,			0.1,		   0,		0.8,		 0.2,		  5,		  5);
	put(wtViewBar,				  1,		   0,		  0,		 0.9,		-20,		  5,		 5,		 5);
	put(wtViewVP,				0.9,		   0,		0.1,		 0.9,		  5,		  5,		20,		 5);
	put(addWaveformButton_,		0.9,		 0.9,		0.1,		 0.1,		  5,		  5,		 5,		 5);
	put(harmEdVP,				0.1,		 0.7,		0.8,		 0.3,		  5,		 40,		 0,		20);
	put(harmEdBar,				0.1,		   1,		0.8,		   0,		  5,		-20,		 0,		 5);
	//put(harmonicsEditor_,		0.1,		 0.7,		0.8,		 0.3,		  5,		 40,		 0,		 5);

	wfEditor_->waveformUpdateSignal().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformUpdateCallback));
	wtView_->waveformRemovedSignal().connect(sigc::mem_fun(this, &CR42YWavetableEditor::waveformUpdateCallback));


}

CR42YWavetableEditor::~CR42YWavetableEditor()
{
}

void CR42YWavetableEditor::waveformAddCallback()
{
	controller_->addWaveform(-1);
	wavetableUpdateCallback();
}

void CR42YWavetableEditor::waveformUpdateCallback()
{
	wfEditor_->queue_draw();
	wfEditor_->updateButtons();
	harmonicsView_->queue_draw();
	wtView_->queue_draw();
}

void CR42YWavetableEditor::wavetableUpdateCallback()
{
	wtView_->queue_draw();
	wtView_->update();
}

void CR42YWavetableEditor::partSelectCallback()
{
	WavetableEditData* data = controller_->data();
	WaveformPart* part = data->getPartByIndex(controller_->selectedWaveform(), controller_->getSelectedPart());

	if (part->getType() == WaveformPart::HARMONICS)
	{
		WPHarmonics* harm = (WPHarmonics*) part;
		harmonicsEditor_->setHarmonicsTable(harm->getHarmonicTable());
	}
}

} /* namespace cr42y */
