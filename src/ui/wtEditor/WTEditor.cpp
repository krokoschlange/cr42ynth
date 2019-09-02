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

#include "WavetableEditData.h"
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
		wtPos(0),
		data(new WavetableEditData(4096)),
		view(nullptr),
		tool(TRI_SLOPE)
{
	/*data->addPart(0, new WPFunction(0.3, 0.6, "x"));
	data->addPart(0, new WPFunction(0.4, 0.5, "-x"));
	
	for (int i = 0; i < 20; i++)
	{
		data->addWaveform();
	}*/
	
	data->removePart(0, 0);
	
	harmonicTable_t ht;
	ht.push_back(std::pair<float, float>(0, 0));
	ht.push_back(std::pair<float, float>(1, 0));
	ht.push_back(std::pair<float, float>(1, 0.5));
	ht.push_back(std::pair<float, float>(0, 0));
	ht.push_back(std::pair<float, float>(0, 0));
	WPHarmonics* hm = new WPHarmonics(0, 1, ht, WPHarmonics::SIN);
	data->addPart(0, hm);

	for (int i = 0; i < data->getWaveforms()->size(); i++)
	{
		selectedParts.push_back(0);
	}
	
	
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
	setWTPos(0);
}

WTEditor::~WTEditor()
{
}

/*int WTEditor::handle(const PuglEvent* event)
{
	if( visible() ) {
		// reverse iter over the children: top first
		for(int i = children.size() - 1; i >= 0 ; i-- ) {
			int ret = children.at( i )->handle( event );
			if( ret ) {
				std::cout << children.at(i)->label() << "\n";
				//AVTK_DEV("widget %s : handles eventType %i ret = 1\n", children.at(i)->label(), event->type );
				return ret; // child widget ate event: done :)
			}
		}

		// if we haven't returned, the event was not consumed by the children, so we
		// can check for a scroll event, and if yes, highlight the next item
		if( event->type == PUGL_SCROLL &&
		    valueMode_ == VALUE_SINGLE_CHILD &&
		    touches( event->scroll.x, event->scroll.y ) &&
		    children.size() > 0 ) {
			// find value() widget
			int vw = -1;
			for(int i = children.size() - 1; i >= 0 ; i-- ) {
				if( children.at(i)->value() > 0.4999 )
					vw = i;
			}

			int d = event->scroll.dy;
			//printf("SCROLL: Value child %i, delta %i\n", vw, d );

			// no widget selected
			if( vw == -1 ) {
				children.at(0)->value( true );
			}
			// scroll up
			else if( vw > 0 && d > 0 ) {
				children.at(vw-1)->value( true  );
				children.at(vw  )->value( false );
			}
			// scroll down
			else if( vw < children.size()-1 && d < 0 ) {
				children.at(vw  )->value( false );
				children.at(vw+1)->value( true  );
			}

			// handled scroll, so eat event
			return 1;
		}
	}
	return 0;
}*/

WavetableEditData* WTEditor::getEditData()
{
	return data;
}

int WTEditor::getWTPos()
{
	return wtPos;
}

void WTEditor::setWTPos(int pos)
{
	pos = pos < 0 ? 0 : pos;
	pos = pos >= data->getWaveforms()->size() ? data->getWaveforms()->size() - 1 :
			pos;
	wtPos = pos;
	if ((*data->getWaveform(pos))[getSelected(pos)]->getType() == WaveformPart::HARMONICS)
	{
		harmEditor->setPart((WPHarmonics*) (*data->getWaveform(pos))[getSelected(pos)]); 
	}
	else
	{
		harmEditor->setPart(nullptr);
	}
	requestRedraw();
}

int WTEditor::getSelected(int row)
{
	if (row >= 0 && row < selectedParts.size())
	{
		return selectedParts[row];
	}
	return -1;
}

void WTEditor::select(int row, int sel)
{
	if (row >= 0 && row < selectedParts.size())
	{
		sel = sel < 0 ? 0 : sel;
		sel = sel >= data->getWaveform(row)->size() ?
				data->getWaveform(row)->size() - 1 : sel;
		selectedParts[row] = sel;
		if ((*data->getWaveform(row))[sel]->getType() == WaveformPart::HARMONICS)
		{
			harmEditor->setPart((WPHarmonics*) (*data->getWaveform(row))[sel]);
		}
		else
		{
			harmEditor->setPart(nullptr);
		}
		requestRedraw();
	}
}

void WTEditor::setTool(TOOL t)
{
	tool = t;
}

int WTEditor::getTool()
{
	return tool;
}

WTTool* WTEditor::getNewTool(float x, float y)
{
	switch (tool)
	{
	case TRI_SLOPE:
		return new TriTool(data, wtPos, x, y);
	case SIN_SLOPE:
		return new SinSlopeTool(data, wtPos, x, y);
	case SIN_HALF:
		return new SinHalfTool(data, wtPos, x, y);
	case FREE:
		return new FreeTool(data, wtPos, x, y);
	default:
		return nullptr;
	}
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

void WTEditor::addWaveform(int idx)
{
	data->addWaveform(idx);
	if (idx == -1 || idx >= selectedParts.size())
	{
		selectedParts.push_back(0);
	}
	else
	{
		selectedParts.insert(selectedParts.begin() + idx, 0);
		if (wtPos >= idx)
		{
			wtPos++;
			requestRedraw();
		}
	}
}

void WTEditor::removeWaveform(int idx)
{
	data->removeWaveform(idx);
	if (idx >= 0 && idx < selectedParts.size())
	{
		selectedParts.erase(selectedParts.begin() + idx);
		if (wtPos > idx)
		{
			wtPos--;
		}
	}
}

void WTEditor::requestRedraw()
{
	view->updateRemoveButtons();
	view->requestRedraw();
	wtView->updateRemoveButtons();
	wtView->requestRedraw();
	harmEditor->requestRedraw();
	harmView->requestRedraw();
	ui->redraw(this);
}

} /* namespace cr42y */
