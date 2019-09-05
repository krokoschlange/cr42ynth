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
#include <cmath>

#include "avtk.hxx"

#include "WaveformView.h"

#include "WTEditor.h"
#include "WavetableEditData.h"
#include "WaveformPart.h"
#include "WTTool.h"
#include "WTView.h"
#include "CRSurfaceButton.h"
#include "WPHarmonics.h"
#include "HarmonicsView.h"

#include "FftRealPair.hpp"

namespace cr42y
{

WaveformView::WaveformView(WTEditor* ed, int x, int y, int w,
		int h, std::string label) :
		Avtk::Group(ed->ui, x, y, w, h, label),
		editor(ed),
		redraw(true),
		surfCache(nullptr),
		cairoCache(nullptr),
		tool(nullptr),
		gridX(0),
		gridY(0),
		resizeLeft(nullptr),
		resizeRight(nullptr)
{
	/*surfCache = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	cairoCache = cairo_create(surfCache);*/
	
	cairo_surface_t* nextPNG = cairo_image_surface_create_from_png("../media/right.png");
	cairo_surface_t* prevPNG = cairo_image_surface_create_from_png("../media/left.png");
	prev = new CRSurfaceButton(ed->ui, x + 5, y + h - 20, 10, 15, "previous",
			prevPNG, 20, 50, prevPNG, 20, 50);
	next = new CRSurfaceButton(ed->ui, x + 20, y + h - 20, 10, 15, "next", 
			nextPNG, 20, 50, nextPNG, 20, 50);
	cairo_surface_destroy(nextPNG);
	cairo_surface_destroy(prevPNG);
	
	cairo_surface_t* plus = cairo_image_surface_create_from_png("../media/plus.png");
	cairo_surface_t* min = cairo_image_surface_create_from_png("../media/minus.png");
	gridXPlus = new CRSurfaceButton(ed->ui, x + w - 20, y + h - 20, 15, 15,
			"gridXPlus", plus, 20, 20, plus, 20, 20);
	gridXMinus = new CRSurfaceButton(ed->ui, x + w - 75, y + h - 20, 15, 15,
			"gridXMinus", min, 20, 20, min, 20, 20);
	gridYPlus = new CRSurfaceButton(ed->ui, x + w - 20, y + h - 40, 15, 15,
			"gridXPlus", plus, 20, 20, plus, 20, 20);
	gridYMinus = new CRSurfaceButton(ed->ui, x + w - 75, y + h - 40, 15, 15,
			"gridXMinus", min, 20, 20, min, 20, 20);
	cairo_surface_destroy(plus);
	cairo_surface_destroy(min);
			
	toSinBtn = new Avtk::Button(ed->ui, x + 35, y + h - 20, 70, 15, "to sin");
	
	add(gridXPlus);
	add(gridXMinus);
	add(gridYPlus);
	add(gridYMinus);
	add(prev);
	add(next);
	add(toSinBtn);

	updateButtons();
}

WaveformView::~WaveformView()
{
	cairo_surface_destroy(surfCache);
	cairo_destroy(cairoCache);
}

void WaveformView::draw(cairo_t* cr)
{
	//TODO: clean up this mess
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
			surfCache = cairo_surface_create_similar(
					cairo_get_target(cr),
					CAIRO_CONTENT_COLOR_ALPHA,
					w(), h());
			cairoCache = cairo_create(surfCache);
		}
		cairo_rectangle(cairoCache, 0, 0, w(), h());
		theme_->color(cairoCache, Avtk::BG_DARK);
		cairo_fill(cairoCache);
		
		for (int i = 0; i < gridX; i++)
		{
			cairo_move_to(cairoCache, ((float) w() / gridX) * i, 0);
			cairo_line_to(cairoCache, ((float) w() / gridX) * i, h());
		}
		if (gridX > 0)
		{
			cairo_move_to(cairoCache, w(), 0);
			cairo_line_to(cairoCache, w(), h());
			theme_->color(cairoCache, Avtk::FG);
			cairo_set_line_width(cairoCache, theme_->lineWidthWide_);
			cairo_stroke(cairoCache);
		}
		
		for (int i = 0; i < gridY; i++)
		{
			cairo_move_to(cairoCache, 0, ((float) h() / gridY) * i);
			cairo_line_to(cairoCache, w(), ((float) h() / gridY) * i);
		}
		if (gridY > 0)
		{
			cairo_move_to(cairoCache, 0, h());
			cairo_line_to(cairoCache, w(), h());
			theme_->color(cairoCache, Avtk::FG);
			cairo_set_line_width(cairoCache, theme_->lineWidthWide_);
			cairo_stroke(cairoCache);
		}

		std::vector<float>* samples = nullptr;
		int stepSize = 1;
		
		if (editor->getEditData())
		{
			stepSize = editor->getEditData()->getWidth() / w();
			stepSize = stepSize > 1 ? stepSize : 1;
			samples = editor->getEditData()->getSamples(editor->getWTPos(), stepSize);
		}
		else
		{
			samples = new std::vector<float>();
			samples->push_back(0);
			samples->push_back(0);
		}

		float pixelPerSample = (float) w() / samples->size();
		
		stepSize = samples->size() / w();
		if (stepSize < 1)
		{
			stepSize = 1;
		}

		for (int i = 1; i < samples->size(); i++)
		{
			cairo_move_to(cairoCache, pixelPerSample * i, h() * -0.5 * (*samples)[i] + h() / 2);
			cairo_line_to(cairoCache, pixelPerSample * (i - stepSize), h() * -0.5 * (*samples)[i - stepSize] + h() / 2);
		}
		cairo_move_to(cairoCache, pixelPerSample * (samples->size() - 1), h() * -0.5 * (*samples)[samples->size() - 1] + h() / 2);
		cairo_line_to(cairoCache, w(), h() * -0.5 * (*samples)[0] + h() / 2);
		cairo_set_line_join(cairoCache, CAIRO_LINE_JOIN_ROUND);
		cairo_set_line_width(cairoCache, theme_->lineWidthWide());
		theme_->color(cairoCache, Avtk::HIGHLIGHT);

		cairo_stroke(cairoCache);

		int sel = editor->getSelected(editor->getWTPos());
		WaveformPart* part = editor->getEditData()->getPartByIndex(editor->getWTPos(), sel);
		std::vector<std::pair<float, float>> parts;
		if (part)
		{
			parts = editor->getEditData()->getVisibleAreas(editor->getWTPos(), part);
		}
		
		for (int i = 0; i < parts.size(); i++)
		{
			cairo_rectangle(cairoCache, parts[i].first * w(), 0, (parts[i].second - parts[i].first) * w(), h());
			theme_->color(cairoCache, Avtk::FG_DARK, 0.4);
			cairo_fill(cairoCache);
			
			if (parts[i].second * w() - parts[i].first * w() < 5)
			{
				cairo_move_to(cairoCache, parts[i].first * w() - 10, 5);
				cairo_line_to(cairoCache, parts[i].first * w(), h() / 2);
				cairo_line_to(cairoCache, parts[i].first * w() - 10, h() - 5);
				cairo_close_path(cairoCache);
				theme_->color(cairoCache, Avtk::FG);
				cairo_fill(cairoCache);
				
				cairo_move_to(cairoCache, parts[i].second * w() + 10, 5);
				cairo_line_to(cairoCache, parts[i].second * w(), h() / 2);
				cairo_line_to(cairoCache, parts[i].second * w() + 10, h() - 5);
				cairo_close_path(cairoCache);
				theme_->color(cairoCache, Avtk::FG);
				cairo_fill(cairoCache);
			}
		}
		
		cairo_set_font_size(cairoCache, 15);
		cairo_select_font_face(cairoCache, "impact", CAIRO_FONT_SLANT_NORMAL,
				CAIRO_FONT_WEIGHT_BOLD);
		cairo_text_extents_t extents;
		theme_->color(cairoCache, Avtk::FG);
		
		cairo_text_extents(cairoCache, "grid X", &extents);
		cairo_move_to(cairoCache, w() - 80 - extents.width, h() - 5);
		cairo_show_text(cairoCache, "gridX");
		
		cairo_text_extents(cairoCache, std::to_string(gridX).c_str(), &extents);
		cairo_move_to(cairoCache, w() - 40 - extents.width / 2, h() - 5);
		cairo_show_text(cairoCache, std::to_string(gridX).c_str());
		
		cairo_text_extents(cairoCache, "grid Y", &extents);
		cairo_move_to(cairoCache, w() - 80 - extents.width, h() - 25);
		cairo_show_text(cairoCache, "gridY");
		
		cairo_text_extents(cairoCache, std::to_string(gridY).c_str(), &extents);
		cairo_move_to(cairoCache, w() - 40 - extents.width / 2, h() - 25);
		cairo_show_text(cairoCache, std::to_string(gridY).c_str());
		
		if (samples)
		{
			delete samples;
		}
	}

	cairo_set_source_surface(cr, surfCache, x(), y());
	cairo_rectangle(cr, x(), y(), w(), h());
	cairo_fill(cr);

	Avtk::Group::draw(cr);

	/*cairo_rectangle(cr, x(), y(), w(), h());
	theme_->color(cr, Avtk::FG);
	cairo_stroke(cr);*/

	cairo_restore(cr);
}

int WaveformView::handle(const PuglEvent* event)
{
	int g = Avtk::Group::handle(event);
	if (g != 0)
	{
		return g;
	}
	if (event->type == PUGL_BUTTON_PRESS && touches(event->button.x, event->button.y) && event->button.button == 3)
	{
		int mx = event->button.x - x();
		int my = event->button.y - y();
		float rmx = (float) mx / w();
		
		WaveformPart* part = editor->getEditData()->getVisiblePartAtPos(editor->getWTPos(), rmx);
		editor->select(editor->getWTPos(), editor->getEditData()->getIndexOfPart(editor->getWTPos(), part));
		updateButtons();
		redraw = true;
		ui->redraw(this);
		return 1;
	}
	else if (event->type == PUGL_BUTTON_PRESS && touches(event->button.x, event->button.y) && event->button.button == 1)
	{
		if (tool)
		{
			delete tool;
			tool = nullptr;
			editor->requestRedraw();
		}
		else
		{
			float snapX = (float) (event->button.x - x()) / w();
			if (gridX > 0)
			{
				snapX = (float) ((int) (snapX * gridX + 0.5)) / gridX;
			}
			float snapY = (float) ((event->button.y - y())) / (h());
			if (gridY > 0)
			{
				snapY = (float) ((int) (snapY * gridY + 0.5)) / gridY;
			}
			snapY = -2 * snapY + 1;
			tool = editor->getNewTool(snapX, snapY);
			editor->select(editor->getWTPos(), editor->getEditData()->getIndexOfPart(editor->getWTPos(), tool->getPart()));
			updateButtons();
			redraw = true;
			ui->redraw(this);
		}
		return 1;
	}
	else if (event->type == PUGL_MOTION_NOTIFY && touches(event->button.x, event->button.y) && tool)
	{
		float snapX = (float) (event->button.x - x()) / w();
		if (gridX > 0)
		{
			snapX = (float) ((int) (snapX * gridX + 0.5)) / gridX;
		}
		float snapY = (float) ((event->button.y - y())) / (h());
		if (gridY > 0)
		{
			snapY = (float) ((int) (snapY * gridY + 0.5)) / gridY;
		}
		snapY = -2 * snapY + 1;
		tool->motion(snapX, snapY);
		redraw = true;
		updateButtons();
		ui->redraw(this);
		//editor->getHarmonicsView()->requestRedraw();
		//editor->requestRedraw();
		return 1;
	}
	else if (event->type == PUGL_BUTTON_RELEASE && event->button.button == 1)
	{
		if (tool)
		{
			delete tool;
		}
		tool = nullptr;
		updateButtons();
		editor->requestRedraw();
		//ui->redraw(editor->getWTView());
	}
	else
	{
		
	}
	
	return 0;
}

void WaveformView::updateButtons()
{
	for (int i = 0; i < removeBtns.size(); i++)
	{
		remove(removeBtns[i].second);
		ui->remove(removeBtns[i].second);
		delete removeBtns[i].second;
	}
	removeBtns.clear();
	if (resizeLeft)
	{
		remove(resizeLeft);
		ui->remove(resizeLeft);
		delete resizeLeft;
	}
	if (resizeRight)
	{
		remove(resizeRight);
		ui->remove(resizeRight);
		delete resizeRight;
	}
	resizeLeft = nullptr;
	resizeRight = nullptr;
	
	std::vector<WaveformPart*>* wf = editor->getEditData()->getWaveform(editor->getWTPos());
	int sel = editor->getSelected(editor->getWTPos());
	if (0 < sel && sel < wf->size())
	{
		//Avtk::Button* rmvBtn = new Avtk::Button(ui, (*wf)[sel]->getStart() * editor->getEditData()->getWidth() + 5, 5, 10, 10, "X");
		std::vector<std::pair<float, float>> visAreas = editor->getEditData()->getVisibleAreas(editor->getWTPos(), (*wf)[sel]);
		if (visAreas.size() > 0)
		{
			cairo_surface_t* min = cairo_image_surface_create_from_png("../media/minus.png");
			int btnX = visAreas[0].first * w() + 5;
			if (btnX + 20 > w())
			{
				btnX = w() - 20;
			}
			CRSurfaceButton* rmvBtn = new CRSurfaceButton(ui,
					x() + btnX,
					y() + 5, 15, 15, "-", min, 20, 20, min, 20, 20);

			removeBtns.push_back(std::pair<int, CRSurfaceButton*>(sel, rmvBtn));
			add(rmvBtn);
			rmvBtn->theme(ui->theme(1));
			cairo_surface_destroy(min);
		}
		
		cairo_surface_t* left = cairo_image_surface_create_from_png("../media/left.png");
		resizeLeft = new CRSurfaceButton(ui,
				x() + (*wf)[sel]->getStart() * w() - 15,
				y() + h() / 2 - 15, 15, 30, "<", left, 20, 50, left, 20, 50);
		add(resizeLeft);
		cairo_surface_destroy(left);
		
		cairo_surface_t* right = cairo_image_surface_create_from_png("../media/right.png");
		resizeRight = new CRSurfaceButton(ui,
				x() + (*wf)[sel]->getEnd() * w(),
				y() + h() / 2 - 15, 15, 30, ">", right, 20, 50, right, 20, 50);
		add(resizeRight);
		cairo_surface_destroy(right);
	}
}

void WaveformView::valueCB(Avtk::Widget* widget)
{
	for (int i = 0; i < removeBtns.size(); i++)
	{
		if (widget == removeBtns[i].second && widget->value())//->value())
		{
			if (tool)
			{
				delete tool;
			}
			tool = nullptr;
			editor->getEditData()->removePart(editor->getWTPos(), removeBtns[i].first);
			int sel = editor->getSelected(editor->getWTPos());
			if (sel < 0)
			{
				sel = 0;
			}
			else if (sel >= editor->getEditData()->getWaveform(editor->getWTPos())->size())
			{
				sel = editor->getEditData()->getWaveform(editor->getWTPos())->size() - 1;
			}
			editor->select(editor->getWTPos(), sel);
			updateButtons();
			redraw = true;
			ui->redraw(this);
			return;
		}
	}
	if (widget == next)
	{
		editor->select(editor->getWTPos(), editor->getSelected(editor->getWTPos()) + 1);
		redraw = true;
		updateButtons();
		ui->redraw(this);
		return;
	}
	if (widget == prev)
	{
		editor->select(editor->getWTPos(), editor->getSelected(editor->getWTPos()) - 1);
		redraw = true;
		updateButtons();
		ui->redraw(this);
		return;
	}
	if (widget == gridXPlus)
	{
		gridX++;
		redraw = true;
		ui->redraw(this);
		return;
	}
	if (widget == gridXMinus)
	{
		gridX = gridX > 1 ? gridX - 1 : 0;
		redraw = true;
		ui->redraw(this);
		return;
	}
	if (widget == gridYPlus)
	{
		gridY++;
		redraw = true;
		ui->redraw(this);
		return;
	}
	if (widget == gridYMinus)
	{
		gridY = gridY > 1 ? gridY - 1 : 0;
		redraw = true;
		ui->redraw(this);
		return;
	}
	if (widget == toSinBtn)
	{
		std::vector<float>* samples = editor->getEditData()->getSamples(editor->getWTPos());
		std::vector<double> realSmpls(samples->begin(), samples->end());
		std::vector<double> imagSmpls(samples->size());
		if (samples)
		{
			delete samples;
		}
		samples = nullptr;
		
		Fft::transform(realSmpls, imagSmpls);
		
		/*realSmpls.erase(realSmpls.begin());
		imagSmpls.erase(imagSmpls.begin());*/
		
		std::vector<WaveformPart*>* wf = editor->getEditData()->getWaveform(editor->getWTPos());
		
		for (int i = 0; i < wf->size(); i++)
		{
			delete (*wf)[i];
		}
		wf->clear();
		
		harmonicTable_t ht;
		
		for (int i = 0; i < 128; i++)
		{
			double real = fabs(realSmpls[i]) < 0.001 ? 0 : realSmpls[i];
			double imag = fabs(imagSmpls[i]) < 0.001 ? 0 : imagSmpls[i];
			float amp = sqrt(real * real + imag * imag) / (realSmpls.size() / 2);
			if (i == 0)
			{
				amp = amp / 2;
			}
			float phase = atan2(-real, -imag) / (2 * M_PI);
			if (phase < 0)
			{
				phase += 1;
			}
			if (amp < 0.001)
			{
				phase = 0;
			}
			ht.push_back(std::pair<float, float>(amp, phase));
		}
		WPHarmonics* harm = new WPHarmonics(0, 1, ht, WPHarmonics::SIN);
		
		wf->push_back(harm);
		editor->select(editor->getWTPos(), 0);
		editor->requestRedraw();
		return;
	}
}

void WaveformView::requestRedraw()
{
	redraw = true;
}

} /* namespace cr42y */
