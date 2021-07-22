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

#include "CR42YAutomationEditor.h"
#include "AutomationEditController.h"
#include "CR42YAutomationSampleEditor.h"
#include "CR42YIntegerEditor.h"
#include "CR42YAutomationSelector.h"
#include "CR42YButton.h"
#include "CR42YVScrollbar.h"
#include "CR42YAutomationSettings.h"

namespace cr42y
{

CR42YAutomationEditor::CR42YAutomationEditor(CR42YUI* ui, CR42YnthCommunicator* communicator) :
		CR42YGrid(ui),
		controller_(new AutomationEditController(communicator)),
		communicator_(communicator),
		sampleEditor_(new CR42YAutomationSampleEditor(ui, controller_)),
		//gridXEditor_(new CR42YIntegerEditor(ui)),
		//gridYEditor_(new CR42YIntegerEditor(ui)),
		selector_(nullptr),
		addBtn_(new CR42YButton(ui)),
		removeBtn_(new CR42YButton(ui)),
		settings_(new CR42YAutomationSettings(ui, controller_))
{
	selector_ = new CR42YAutomationSelector(ui, controller_);
	
	configureColumn(1, 1, 0, 0, 0, 15);
	configureColumn(2, 1, 0, 0, 0, 0);
	configureColumn(3, 1, 0, 0, 0, 15);
	configureColumn(4, 12, 0, 0, 0, 0);
	configureColumn(5, 4, 0, 0, 0, 0);
	configureRow(0, 2, 0, 0, 0, 0);
	configureRow(1, 1, 0, 0, 0, 0);
	configureRow(2, 1, 0, 0, 0, 0);
	
	addBtn_->setText("Add");
	removeBtn_->setText("Remove");
	
	CR42YVScrollbar* selectorScrollbar = new CR42YVScrollbar(ui);
	Gtk::Adjustment selectorHadj(0, 0, 0, 0, 0, 0);
	Gtk::Viewport* selectorViewport = new Gtk::Viewport(selectorHadj, *selectorScrollbar->get_adjustment());
	selectorViewport->signal_scroll_event().connect(sigc::mem_fun(selectorScrollbar, &CR42YVScrollbar::scroll_event));
	selectorViewport->set_shadow_type(Gtk::SHADOW_NONE);
	selectorViewport->add(*selector_);
	
	put(selectorViewport, 0, 0, 2, 3);
	put(selectorScrollbar, 0, 3, 2, 1);
	put(addBtn_, 2, 0, 1, 2);
	put(removeBtn_, 2, 2, 1, 2);
	put(sampleEditor_, 0, 4, 3, 1);
	put(settings_, 0, 5, 3, 1);
	//put(gridXEditor_, 0, 4);
	//put(gridYEditor_, 1, 4, 2);
	
	//gridXEditor_->setMax(0, false);
	//gridYEditor_->setMax(0, false);
	
	//gridXEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YAutomationEditor::gridChange));
	//gridYEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YAutomationEditor::gridChange));
	settings_->signalGridX().connect(sigc::mem_fun(this, &CR42YAutomationEditor::gridXChange));
	settings_->signalGridY().connect(sigc::mem_fun(this, &CR42YAutomationEditor::gridYChange));
	
	controller_->selectionChangedSignal().connect(sigc::mem_fun(this, &CR42YAutomationEditor::selectionChange));
	controller_->dataChangedSignal().connect(sigc::mem_fun(this, &CR42YAutomationEditor::dataChange));
	
	addBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YAutomationEditor::addCallback));
	removeBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YAutomationEditor::removeCallback));
	
	selector_->signalSelected().connect(sigc::mem_fun(this, &CR42YAutomationEditor::selected));
	
	settings_->update();
}

CR42YAutomationEditor::~CR42YAutomationEditor()
{
	delete controller_;
	delete selector_;
	std::vector<Gtk::Widget*> children = get_children();
	
	for (size_t i = 0; i < children.size(); i++)
	{
		remove(*(children[i]));
		delete children[i];
	}
}

void CR42YAutomationEditor::gridXChange(int grid)
{
	sampleEditor_->setGridX(grid);
}

void CR42YAutomationEditor::gridYChange(int grid)
{
	sampleEditor_->setGridY(grid);
}

void CR42YAutomationEditor::selectionChange(size_t selected)
{
	queue_draw();
	sampleEditor_->queue_draw();
	selector_->update();
	selector_->select(selected, false);
	selector_->queue_draw();
	settings_->update();
}

void CR42YAutomationEditor::selected(int selection)
{
	controller_->selectAutomation((size_t) selection);
}

void CR42YAutomationEditor::dataChange()
{
	queue_draw();
	sampleEditor_->queue_draw();
	selector_->update();
	selector_->queue_draw();
}

void CR42YAutomationEditor::addCallback()
{
	controller_->createAutomation();
}

void CR42YAutomationEditor::removeCallback()
{
	controller_->removeAutomation(controller_->getID(controller_->selectedAutomation()));
}

}
