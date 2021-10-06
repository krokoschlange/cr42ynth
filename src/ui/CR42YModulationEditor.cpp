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

#include "CR42YModulationEditor.h"

#include "CR42YAutomationDial.h"
#include "CR42YSquareContainer.h"
#include "CR42YTheme.h"
#include "CR42YToggle.h"
#include "CR42YToggleSelector.h"
#include "ModulationControls.h"
#include "CR42YAutomationOverview.h"
#include "CR42YVScrollbar.h"

#include "common.h"

namespace cr42y
{

	CR42YModulationEditor::CR42YModulationEditor(CR42YUI* ui, CR42YnthCommunicator* communicator, AutomationEditController* automationEditController) :
	CR42YGrid(ui),
	outerSquare_(new CR42YSquareContainer()),
	grid_(new CR42YGrid(ui)),
	modSelector_(new CR42YToggleSelector(ui)),
	modSquare_(new CR42YSquareContainer()),
	modGrid_(new CR42YGrid(ui)),
	automations_(new CR42YAutomationOverview(ui, automationEditController)),
	automScrollbar_(new CR42YVScrollbar(ui)),
	automVP_(nullptr),
	controls_(new ModulationControls(communicator))
{
	configureRow(0, 1, 0, 0, 0, 0);
	configureColumn(0, 1, 0, 0, 0, 0);
	configureColumn(1, 1, 0, 0, 0, 15);
	configureColumn(2, 5, 0, 0, 0, 0);
	configureColumn(3, 1, 0, 0, 0, 15);
	configureColumn(4, 1, 0, 0, 0, 0);
	
	put(outerSquare_, 0, 2, 1, 1);
	
	Gtk::Adjustment hAdj(0, 0, 0, 0, 0, 0);
	automVP_ = new Gtk::Viewport(hAdj, *automScrollbar_->get_adjustment());
	automVP_->signal_scroll_event().connect(sigc::mem_fun(automScrollbar_, &CR42YVScrollbar::scroll_event));
	automVP_->set_shadow_type(Gtk::SHADOW_NONE);
	automVP_->add(*automations_);
	
	put(automVP_, 0, 0, 1, 1);
	put(automScrollbar_, 0, 1, 1, 1);
	
	outerSquare_->add(*grid_);

	grid_->configureRow(0, 1, 0.002, 0.002, 0, 0);
	grid_->configureRow(1, 8, 0.002, 0.002, 0, 0);
	grid_->configureColumn(0, 1, 0, 0, 0, 0);

	CR42YToggle* tgl = new CR42YToggle(ui);
	tgl->setText("AM");
	tgl->setFontSize(CR42YTheme::MIDDLE);
	modSelector_->putToggle(tgl, 0, 0, 0.25, 1);
	tgl = new CR42YToggle(ui);
	tgl->setText("FM");
	tgl->setFontSize(CR42YTheme::MIDDLE);
	modSelector_->putToggle(tgl, 0.25, 0, 0.25, 1);
	tgl = new CR42YToggle(ui);
	tgl->setText("PM");
	tgl->setFontSize(CR42YTheme::MIDDLE);
	modSelector_->putToggle(tgl, 0.5, 0, 0.25, 1);
	tgl = new CR42YToggle(ui);
	tgl->setText("RM");
	tgl->setFontSize(CR42YTheme::MIDDLE);
	modSelector_->putToggle(tgl, 0.75, 0, 0.25, 1);
	
	modSelector_->select(0);
	
	modSelector_->signalSelected().connect(sigc::mem_fun(this, &CR42YModulationEditor::selectCallback));
	grid_->put(modSelector_, 0, 0, 1, 1);
	grid_->put(modSquare_, 1, 0, 1, 1);

	modSquare_->add(*modGrid_);

	modGrid_->configureRow(0, 0.7, 0.002, 0.002, 0, 0);
	modGrid_->configureColumn(0, 0.7, 0.002, 0.002, 0, 0);

	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		modGrid_->addRow(1, 0.002, 0.002, 0, 0);
		modGrid_->addColumn(1, 0.002, 0.002, 0, 0);
	}

	modGrid_->addRow(0.7, 0.002, 0.002, 0, 0);
	modGrid_->addColumn(0.7, 0.002, 0.002, 0, 0);

	CR42YLabel* lbl = new CR42YLabel(ui);
	lbl->setText("CARRIER");
	lbl->setDrawBorder(false);
	modGrid_->put(lbl, 0, 1, 1, CR42Ynth_OSC_COUNT);
	lbl = new CR42YLabel(ui);
	lbl->setText("MOD");
	lbl->setDrawBorder(false);
	modGrid_->put(lbl, 1, 0, CR42Ynth_OSC_COUNT, 1);

	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		lbl = new CR42YLabel(ui);
		lbl->setText(std::to_string(i + 1));
		lbl->setDrawBorder(false);
		modGrid_->put(lbl, CR42Ynth_OSC_COUNT + 1, 1 + i, 1, 1);
		lbl = new CR42YLabel(ui);
		lbl->setText(std::to_string(i + 1));
		lbl->setDrawBorder(false);
		modGrid_->put(lbl, 1 + i, CR42Ynth_OSC_COUNT + 1, 1, 1);
	}

	for (int mod = 0; mod < CR42Ynth_OSC_COUNT; mod++)
	{
		for (int carr = 0; carr < CR42Ynth_OSC_COUNT; carr++)
		{
			CR42YAutomationDial* dial = new CR42YAutomationDial(ui);
			modGrid_->put(dial, mod + 1, carr + 1, 1, 1);
			dials_.push_back(dial);
		}
	}

	connectControls();
}

CR42YModulationEditor::~CR42YModulationEditor()
{
	std::vector<Gtk::Widget*> children = modGrid_->get_children();

	for (size_t i = 0; i < children.size(); i++)
	{
		delete children[i];
	}

	delete modGrid_;
	children = grid_->get_children();

	for (size_t i = 0; i < children.size(); i++)
	{
		delete children[i];
	}

	//modSquare_ and modSelector_ are already deleted here
	delete grid_;
	delete outerSquare_;
	delete automations_;
}

void CR42YModulationEditor::connectControls()
{
	Control** controls = controls_->amControls;

	switch (modSelector_->selected())
	{
	case 0:
		controls = controls_->amControls;
		break;
	case 1:
		controls = controls_->fmControls;
		break;
	case 2:
		controls = controls_->pmControls;
		break;
	case 3:
		controls = controls_->rmControls;
		break;
	default:
		break;
	}

	for (int mod = 0; mod < CR42Ynth_OSC_COUNT; mod++)
	{
		for (int carr = 0; carr < CR42Ynth_OSC_COUNT; carr++)
		{
			dials_[mod * CR42Ynth_OSC_COUNT + carr]->connectControl(controls[mod * CR42Ynth_OSC_COUNT + carr]);
		}
	}
}

void CR42YModulationEditor::selectCallback(int)
{
	connectControls();
}

}
