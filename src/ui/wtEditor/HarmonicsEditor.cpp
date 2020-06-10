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

#include "button.hxx"
#include "theme.hxx"
#include "ui.hxx"

#include "HarmonicsEditor.h"

#include "HarmonicsView.h"
#include "WaveformView.h"
#include "WTEditor.h"
#include "WTView.h"
#include "DragBar.h"
#include "WPHarmonics.h"
#include "CustomScroll.h"

#include "WavetableEditData.h"
#include "WavetableEditController.h"

namespace cr42y
{

HarmonicsEditor::HarmonicsEditor(WTEditor* ed, int x, int y, int w, int h,
		std::string label) :
		barWidth(25),
		Avtk::Group(ed->ui, x, y, 129 * 25, h - 10, label),
		editor(ed),
		scroll(new CustomScroll(ed->ui, x, y, w, h, label + "_scroll")),
		part(nullptr),
		redraw(true),
		surfCache(nullptr),
		cairoCache(nullptr)
{
	//h_ = 300;
	for (int i = 0; i < 129; i++)
	{
		DragBar* aBar = new DragBar(ed->ui, x + i * barWidth, y, barWidth, (h_ - 15) / 2, "", true);
		DragBar* pBar = new DragBar(ed->ui, x + i * barWidth, y + (h_ - 15) / 2 + 15, barWidth, (h - 15) / 2, "", true);
		amplitudes.push_back(aBar);
		phases.push_back(pBar);
		add(aBar);
		add(pBar);
	}
	
	normButton = new Avtk::Button(ed->ui, x + w - 85, y + 5, 80, 22, "normalize");
	
	ed->add(scroll);
	scroll->setChild(this);
	scroll->childResize();
	ui->redraw(scroll);
	ed->add(normButton);
	//children.push_back(normButton);
	normButton->callback = staticGroupCB;
	normButton->callbackUD = this;
}

HarmonicsEditor::~HarmonicsEditor()
{
	if (surfCache)
	{
		cairo_surface_destroy(surfCache);
	}
	if (cairoCache)
	{
		cairo_destroy(cairoCache);
	}
}

void HarmonicsEditor::draw(cairo_t* cr)
{
	cairo_save(cr);
	
	WavetableEditController* controller = editor->getController();
	WavetableEditData* data = controller->getData();
	if (data)
	{
		WaveformPart* p = data->getPartByIndex(controller->getSelectedWaveform(), controller->getSelectedPart());
		if (p && p->getType() == WaveformPart::HARMONICS && p != part)
		{
			part = (WPHarmonics*) p;
			requestRedraw();
		}
		else if (!p || p->getType() != WaveformPart::HARMONICS)
		{
			part = nullptr;
			requestRedraw();
		}
	}

	if (redraw)
	{
		redraw = false;
		if (!surfCache)
		{
			if (cairoCache)
			{
				cairo_destroy(cairoCache);
			}
			surfCache = cairo_surface_create_similar(cairo_get_target(cr), CAIRO_CONTENT_COLOR_ALPHA, w(), h());
			cairoCache = cairo_create(surfCache);
		}
		cairo_save(cairoCache);
		
		//cairo_rectangle(cairoCache, 0, y(), w(), h());
		theme_->color(cairoCache, Avtk::BG);
		cairo_paint(cairoCache);
		
		Avtk::Group::draw(cairoCache);
		
		for (int i = 0; i < 129; i++)
		{
			cairo_text_extents_t extents;
			std::string str = i != 0 ? std::to_string(i) : "DC";
			cairo_text_extents(cairoCache, str.c_str(), &extents);
			cairo_move_to(cairoCache, barWidth * i + barWidth / 2 - extents.width / 2, (h() - 15) / 2 + 15 / 2 + extents.height / 2);
			cairo_select_font_face(cairoCache, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
			cairo_set_font_size(cairoCache, 10);
			theme_->color(cairoCache, Avtk::FG);
			cairo_show_text(cairoCache, str.c_str());
			
		}
		
		if (!part)
		{
			normButton->visible(false);
			cairo_rectangle(cairoCache, x(), y(), w(), h());
			theme_->color(cairoCache, Avtk::BG, 0.9);
			cairo_fill(cairoCache);
		}
		else
		{
			normButton->visible(true);
		}
		
		cairo_restore(cairoCache);
	}
	
	cairo_set_source_surface(cr, surfCache, x(), y());
	cairo_paint(cr);
	
	cairo_restore(cr);
}

int HarmonicsEditor::handle(const PuglEvent* event)
{
	if (!visible())
	{
		return 0;
	}
	if (part)
	{
		if (event->type == PUGL_BUTTON_PRESS || event->type == PUGL_BUTTON_RELEASE || event->type == PUGL_MOTION_NOTIFY)
		{
			int g = Avtk::Group::handle(event);
			if (event->type == PUGL_BUTTON_RELEASE)
			{
				editor->requestRedraw();
			}
			if (g)
			{
				return g;
			}
		}
	}
	return 0;
}

void HarmonicsEditor::valueCB(Avtk::Widget* widget)
{
	if (!part)
	{
		return;
	}
	if (widget == normButton)
	{
		part->normalize();
		editor->getWFView()->requestRedraw();
		editor->getWTView()->requestRedraw();
		editor->getHarmonicsView()->requestRedraw();
		requestRedraw();
		setPart(part);

		return;
	}
	for (int i = 0; i < amplitudes.size(); i++)
	{
		if (widget == amplitudes[i])
		{
			harmonicTable_t* ht = part->getHarmonicTable();
			if (ht->size() > i)
			{
				(*ht)[i].first = widget->value() * 2 - 1;
				part->setUpdate();
				//part->normalize();
				editor->getWFView()->requestRedraw();
				editor->getHarmonicsView()->requestRedraw();
				requestRedraw();
			}
			//editor->requestRedraw();
			return;
		}
	}
	for (int i = 0; i < phases.size(); i++)
	{
		if (widget == phases[i])
		{
			harmonicTable_t* ht = part->getHarmonicTable();
			if (ht->size() > i)
			{
				(*ht)[i].second = widget->value() * 2 - 1;
				part->setUpdate();
				//part->normalize();
				editor->getWFView()->requestRedraw();
				editor->getHarmonicsView()->requestRedraw();
				requestRedraw();
			}
			return;
		}
	}
	
	//editor->requestRedraw();
}

void HarmonicsEditor::setPart(WPHarmonics* p)
{
	part = p;
	if (part)
	{
		harmonicTable_t* ht = part->getHarmonicTable();
		for (int i = 0; i < ht->size() && i < amplitudes.size(); i++)
		{
			amplitudes[i]->value(((*ht)[i].first + 1) / 2);
			phases[i]->value(((*ht)[i].second + 1) / 2);
		}
	}
	requestRedraw();
}

void HarmonicsEditor::requestRedraw()
{
	redraw = true;
}

} /* namespace cr42y */
