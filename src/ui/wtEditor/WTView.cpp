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

#include "avtk.hxx"

#include "WTView.h"

#include "WTEditor.h"
#include "CustomScroll.h"
#include "CRSurfaceButton.h"
#include "CR42YnthUI.h"

#include "WavetableEditController.h"

namespace cr42y
{

WTView::WTView(WTEditor* ed, int x, int y, int w, int h, std::string label) :
		Avtk::Group(ed->ui, x, y, w - 10, h, label),
		editor(ed),
		scroll(new CustomScroll(ed->ui, x, y, w, h, label + "_scroll")),
		boxSize(50),
		redraw(true),
		surfCache(nullptr),
		cairoCache(nullptr)
{
	ed->add(scroll);
	scroll->setChild(this);
	
	/*surfCache = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	 cairoCache = cairo_create(surfCache);*/
	updateRemoveButtons();
	
	std::string path = std::string(((CR42YnthUI*) ui)->getBundlePath());

	cairo_surface_t* plus = cairo_image_surface_create_from_png((path + "../media/plus.png").c_str());
	addBtn = new CRSurfaceButton(ed->ui, x + 5, y + h - 30, 25, 25, "add", plus, 20, 20, plus, 20, 20);
	cairo_surface_destroy(plus);
	ed->add(addBtn);
	addBtn->callback = staticGroupCB;
	addBtn->callbackUD = this;
}

WTView::~WTView()
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

void WTView::draw(cairo_t* cr)
{
	cairo_save(cr);
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
		cairo_rectangle(cairoCache, 0, 0, w(), h());
		theme_->color(cairoCache, Avtk::BG_DARK);
		cairo_fill(cairoCache);

		//WavetableEditData* editData = editor->getEditData();
		WavetableEditController* controller = editor->getController();
		//std::vector<std::vector<float>>* samples = editData->getSamples();
		for (int i = 0; i < controller->getWavetableHeight(); i++)
		{
			cairo_rectangle(cairoCache, 0, boxSize * i, w(), boxSize);
			theme_->color(cairoCache, Avtk::BG);
			cairo_stroke_preserve(cairoCache);
			cairo_clip(cairoCache);
			if (controller->getSelectedWaveform() == i)
			{
				cairo_rectangle(cairoCache, 0, boxSize * i, w(), boxSize);
				theme_->color(cairoCache, Avtk::HIGHLIGHT, 0.4);
				cairo_fill(cairoCache);
			}
			

			int stepSize = controller->getWaveformWidth() / w();
			if (stepSize < 1)
			{
				stepSize = 1;
			}
			std::vector<float>* samples = controller->getSamples(i, stepSize);
			float ppS = (float) w() / samples->size();
			for (int j = 0; j < samples->size(); j++)
			{
				cairo_move_to(cairoCache, ppS * j, boxSize * i + boxSize * 0.5 - (boxSize - theme_->lineWidthWide()) * 0.5 * (*samples)[j]);
				cairo_line_to(cairoCache, ppS * (j - 1), boxSize * i + boxSize * 0.5 - (boxSize - theme_->lineWidthWide()) * 0.5 * (*samples)[j - 1]);
			}
			cairo_move_to(cairoCache, ppS * (controller->getWaveformWidth() - 1), boxSize * i + boxSize * 0.5 - (boxSize - theme_->lineWidthWide()) * 0.5 * (*samples)[controller->getWaveformWidth() - 1]);
			cairo_line_to(cairoCache, w(), boxSize * i + boxSize * 0.5 - (boxSize - theme_->lineWidthWide()) * 0.5 * (*samples)[0]);
			//cairo_set_line_join(cairoCache, CAIRO_LINE_JOIN_ROUND);
			cairo_set_line_width(cairoCache, theme_->lineWidthWide());
			theme_->color(cairoCache, Avtk::FG_DARK);
			cairo_stroke(cairoCache);
			cairo_reset_clip(cairoCache);
			if (samples)
			{
				delete samples;
			}
			samples = nullptr;
		}

	}
	cairo_rectangle(cr, x(), y(), w(), h());
	theme_->color(cr, Avtk::BG_DARK);
	cairo_fill_preserve(cr);
	cairo_set_source_surface(cr, surfCache, x(), y());
	
	cairo_fill(cr);
	
	Avtk::Group::draw(cr);
	
	cairo_restore(cr);
}

int WTView::handle(const PuglEvent* event)
{
	if (!visible())
	{
		return 0;
	}
	int g = Group::handle(event);
	if (g)
	{
		return g;
	}
	if (event->type == PUGL_BUTTON_PRESS && event->button.button == 1 && touches(event->button.x, event->button.y))
	{
		int sel = event->button.y / boxSize;
		WavetableEditController* controller = editor->getController();
		controller->selectWaveform(sel);
		return 1;
	}
	return 0;
}

void WTView::valueCB(Avtk::Widget* widget)
{
	//WavetableEditData* editData = editor->getEditData();
	WavetableEditController* controller = editor->getController();
	//std::vector<std::vector<WaveformPart*>>* wf = editData->getWaveforms();
	if (widget == addBtn)
	{
		controller->addWaveform(-1);

		int oldH = h();
		h(controller->getWavetableHeight() * boxSize);
		if (oldH != h())
		{
			cairo_surface_destroy(surfCache);
			cairo_destroy(cairoCache);
			cairoCache = nullptr;
			surfCache = nullptr;
			scroll->childResize();
		}

		editor->requestRedraw();
		return;
	}
	for (int i = 0; i < removeBtns.size(); i++)
	{
		if (removeBtns[i] == widget && widget->value())
		{
			controller->removeWaveform(i);
			
			int oldH = h();
			h(controller->getWavetableHeight() * boxSize);
			if (oldH != h())
			{
				cairo_surface_destroy(surfCache);
				cairo_destroy(cairoCache);
				cairoCache = nullptr;
				surfCache = nullptr;
				scroll->childResize();
			}

			editor->requestRedraw();
			return;
		}
	}
}

void WTView::updateRemoveButtons()
{
	for (int i = 0; i < removeBtns.size(); i++)
	{
		remove(removeBtns[i]);
		ui->remove(removeBtns[i]);
		delete (removeBtns[i]);
	}
	removeBtns.clear();
	WavetableEditController* controller = editor->getController();
	//std::vector<std::vector<WaveformPart*>>* wt = editor->getEditData()->getWaveforms();
	std::string path = std::string(((CR42YnthUI*) ui)->getBundlePath());
	for (int i = 0; i < controller->getWavetableHeight(); i++)
	{
		cairo_surface_t* min = cairo_image_surface_create_from_png((path + "../media/minus.png").c_str());
		CRSurfaceButton* btn = new CRSurfaceButton(ui, x() + 5, y() + (i * boxSize) + 5, 15, 15, "rmv", min, 20, 20, min, 20, 20);
		removeBtns.push_back(btn);
		add(btn);
		cairo_surface_destroy(min);
	}
}

void WTView::requestRedraw()
{
	redraw = true;
}

} /* namespace cr42y */
