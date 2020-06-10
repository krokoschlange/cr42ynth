/*******************************************************************************
 * Copyright (c) 2019 krokoschlange and contributors.
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


#include <iostream>

#include "ui.hxx"

#include "WTEditor.h"
#include "TriTool.h"
#include "SinSlopeTool.h"
#include "SinHalfTool.h"
#include "FreeTool.h"
#include "WPFunction.h"
#include "WPHarmonics.h"

#include "WavetableEditController.h"
#include "WaveformView.h"
#include "HarmonicsEditor.h"
#include "HarmonicsView.h"
#include "WTView.h"
#include "WTTool.h"
#include "ToolPanel.h"

namespace cr42y
{

WTEditor::WTEditor(Avtk::UI* ui, int x, int y, int w, int h, std::string label) :
		Avtk::Group(ui, x, y, w, h, label),
		view(nullptr),
		controller(new WavetableEditController())
{
	/*data->addPart(0, new WPFunction(0.3, 0.6, "x"));
	data->addPart(0, new WPFunction(0.4, 0.5, "-x"));

	for (int i = 0; i < 20; i++)
	{
		data->addWaveform();
	}*/

	/*data->removePart(0, 0);

	harmonicTable_t ht;
	ht.push_back(std::pair<float, float>(0, 0));
	ht.push_back(std::pair<float, float>(1, 0));
	WPHarmonics* hm = new WPHarmonics(0, 1, ht, WPHarmonics::SIN);
	data->addPart(0, hm);

	for (int i = 0; i < data->getWaveforms()->size(); i++)
	{
		selectedParts.push_back(0);
	}*/


	int pad = w / 100;
	
	int usableW = w - 4 * pad;
	

	view = new WaveformView(this, x + usableW / 6 + pad * 2, y + pad * 2 + h * 0.15, usableW / 1.5, h * 0.5, "VIEW");
	add(view);
	wtView = new WTView(this, x + usableW / 6 + usableW / 1.5 + 3* pad, y + pad, usableW / 6, h - 2 * pad, "WTView");
	toolPanel = new ToolPanel(this, pad, pad, usableW / 6, h - 2 * pad, "TP");
	add(toolPanel);
	harmEditor = new HarmonicsEditor(this, x + usableW / 6 + pad * 2, y + pad * 3 + h * 0.65, usableW / 1.5,
			h - pad * 4 - h * 0.65, "HEDIT");
	harmView = new HarmonicsView(this, x + usableW / 6 + pad * 2, y + pad, usableW / 1.5, h * 0.15, "HVIEW");
	add(harmView);
	//setWTPos(0);
}

WTEditor::~WTEditor()
{
	if (controller)
	{
		delete controller;
	}
}

WavetableEditController* WTEditor::getController()
{
	return controller;
}

WaveformView* WTEditor::getWFView()
{
	return view;
}

WTView* WTEditor::getWTView()
{
	return wtView;
}

ToolPanel* WTEditor::getToolPanel()
{
	return toolPanel;
}

HarmonicsEditor* WTEditor::getHarmonicsEditor()
{
	return harmEditor;
}

HarmonicsView* WTEditor::getHarmonicsView()
{
	return harmView;
}

void WTEditor::requestRedraw()
{
	view->updateButtons();
	view->requestRedraw();
	wtView->updateRemoveButtons();
	wtView->requestRedraw();
	harmEditor->requestRedraw();
	harmView->requestRedraw();
	toolPanel->updateButtons();
	ui->redraw();
}

} /* namespace cr42y */
