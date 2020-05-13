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
#include <cairo/cairo.h>
#include "theme.hxx"
#include "ui.hxx"

#include "ToolPanel.h"

#include "WTEditor.h"
#include "CRSurfaceButton.h"
#include "WavetableEditData.h"
#include "WaveformPart.h"
#include "WPFunction.h"
#include "WPHarmonics.h"

namespace cr42y
{
ToolPanel::ToolPanel(WTEditor* ed, int x, int y, int w, int h, std::string label) :
		Avtk::Group(ed->ui, x, y, w, h, label),
		editor(ed),
		baseType(0)
{
	int pad = w / 20;
	int btnH = 50;
	int btnW = (w - 4 * pad) / 3;
	
	
	cairo_surface_t* png = cairo_image_surface_create_from_png("../media/linCurve.png");
	CRSurfaceButton* btn = new CRSurfaceButton(ed->ui, x + pad, y + pad, btnW, btnH,
			"linear", png, 50, 50, png, 50, 50);
	cairo_surface_destroy(png);
	toolBtns.push_back(btn);
	btn->value(1);
	
	png = cairo_image_surface_create_from_png("../media/sinCurve.png");
	btn = new CRSurfaceButton(ed->ui, x + pad * 2 + btnW, y + pad,
			btnW, btnH, "sin ramp", png, 50, 50, png, 50, 50);
	cairo_surface_destroy(png);
	toolBtns.push_back(btn);
	
	png = cairo_image_surface_create_from_png("../media/sinHalfCurve.png");
	btn = new CRSurfaceButton(ed->ui, x + pad * 3 + btnW * 2, y + pad,
			btnW, btnH, "sin half", png, 50, 50, png, 50, 50);
	cairo_surface_destroy(png);
	toolBtns.push_back(btn);
	
	png = cairo_image_surface_create_from_png("../media/freeDraw.png");
	btn = new CRSurfaceButton(ed->ui, x + pad, y + pad * 2 + btnH,
			btnW, btnH, "free", png, 50, 50, png, 50, 50);
	cairo_surface_destroy(png);
	toolBtns.push_back(btn);
	
	
	/*toolBtns.push_back(triBtn);
	toolBtns.push_back(sinRampBtn);
	toolBtns.push_back(sinHalfBtn);
	toolBtns.push_back(freeBtn);*/
	
	for (int i = 0; i < toolBtns.size(); i++)
	{
		toolBtns[i]->label_visible = true;
		toolBtns[i]->clickMode(CLICK_TOGGLE);
		add(toolBtns[i]);
	}
	
	int selBtnH = btnH / 1.5;
	int selBtnW = (w - 2 * pad) / 3;
	
	Avtk::Button* selBtn= new Avtk::Button(ed->ui, x + pad,
			y + pad * 4 + btnH * 2, selBtnW, selBtnH,
			"HARM");
	baseTypeSelector.push_back(selBtn);
	selBtn->value(1);
	
	selBtn= new Avtk::Button(ed->ui, x + pad + selBtnW,
			y + pad * 4 + btnH * 2, selBtnW, selBtnH,
			"f(x)");
	baseTypeSelector.push_back(selBtn);
	
	selBtn= new Avtk::Button(ed->ui, x + pad + 2 * selBtnW,
			y + pad * 4 + btnH * 2, selBtnW, selBtnH,
			"WAV");
	baseTypeSelector.push_back(selBtn);
	
	for (int i = 0; i < baseTypeSelector.size(); i++)
	{
		baseTypeSelector[i]->clickMode(CLICK_TOGGLE);
		add(baseTypeSelector[i]);
	}
}

ToolPanel::~ToolPanel()
{
	
}

int ToolPanel::handle(const PuglEvent* event)
{
	int g = Avtk::Group::handle(event);
	/*toolBtns[editor->getTool()]->value(1);
	ui->redraw(this);*/
	if (event->type == PUGL_BUTTON_PRESS &&
			touches(event->button.x, event->button.y))
	{
		toolBtns[editor->getTool()]->value(1);
		baseTypeSelector[baseType]->value(1);
		ui->redraw(this);
	}
	return g;
}

void ToolPanel::valueCB(Avtk::Widget* widget)
{
	for (int i = 0; i < toolBtns.size(); i++)
	{
		if (widget == toolBtns[i])
		{
			for (int j = 0; j < toolBtns.size(); j++)
			{
				if (toolBtns[j] != widget)
				{
					toolBtns[j]->value(0);
				}
			}
			editor->setTool((WTEditor::TOOL) i);
			return;
		}
	}
	for (int i = 0; i < baseTypeSelector.size(); i++)
	{
		if (widget == baseTypeSelector[i])
		{
			for (int j = 0; j < baseTypeSelector.size(); j++)
			{
				if (baseTypeSelector[j] != widget)
				{
					baseTypeSelector[j]->value(0);
				}
			}
			if (i != baseType)
			{
				baseType = i;
				editor->getEditData()->removePart(editor->getWTPos(), 0);
				WaveformPart* newBase = nullptr;
				switch (i)
				{
				case 0:
				{
					harmonicTable_t ht;
					ht.push_back(std::pair<float, float>(0, 0));
					ht.push_back(std::pair<float, float>(1, 0));
					newBase = new WPHarmonics(0, 1, ht, WPHarmonics::SIN);
					break;
				}
				default:
					newBase = new WPFunction(0, 1, "0");
				}
				editor->getEditData()->addPart(editor->getWTPos(), newBase, 0);
				editor->select(editor->getWTPos(), editor->getSelected(editor->getWTPos()));
				editor->requestRedraw();
			}
			return;
		}
	}
}



} /* namespace cr42y */
