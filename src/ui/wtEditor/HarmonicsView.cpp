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

#include "theme.hxx"
#include "ui.hxx"

#include "HarmonicsView.h"

#include "WTEditor.h"
#include "WavetableEditData.h"

#include "FftRealPair.hpp"

namespace cr42y
{

HarmonicsView::HarmonicsView(WTEditor* ed, int x, int y, int w, int h, std::string label) :
		Avtk::Widget(ed->ui, x, y, w, h, label),
		editor(ed),
		redraw(true),
		surfCache(nullptr),
		cairoCache(nullptr)
{
	//surfCache = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	//cairoCache = cairo_create(surfCache);
}

HarmonicsView::~HarmonicsView()
{
	
}

void HarmonicsView::draw(cairo_t* cr)
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
			surfCache = cairo_surface_create_similar(
					cairo_get_target(cr),
					CAIRO_CONTENT_COLOR_ALPHA,
					w(), h());
			cairoCache = cairo_create(surfCache);
		}
		cairo_save(cairoCache);
		
		cairo_rectangle(cairoCache, 0, 0, w(), h());
		theme_->color(cairoCache, Avtk::BG_DARK);
		cairo_fill(cairoCache);
		
		std::vector<float>* samples = editor->getEditData()->getSamples(editor->getWTPos());
		std::vector<double> doubleSmpls(samples->begin(), samples->end());
		delete samples;
		std::vector<double> doubleEmpty(doubleSmpls.size());
		
		Fft::transform(doubleSmpls, doubleEmpty);
		
		//doubleSmpls.erase(doubleSmpls.begin());
		//doubleEmpty.erase(doubleEmpty.begin());
		
		float maxAmp = 0;
		for (int i = 0; i < 129; i++)
		{
			float amp = sqrt(doubleSmpls[i] * doubleSmpls[i] +
					doubleEmpty[i] * doubleEmpty[i]) / (doubleSmpls.size() / 2);
			if (amp > maxAmp)
			{
				maxAmp = amp;
			}
		}
		
		float ampHeight = h() / 1.5;
		
		theme_->color(cairoCache, Avtk::HIGHLIGHT);
		for (int i = 0; i < 129; i++)
		{
			float amp = sqrt(doubleSmpls[i] * doubleSmpls[i] +
					doubleEmpty[i] * doubleEmpty[i]) / (doubleSmpls.size() / 2) / maxAmp;
					
			if (i == 0)
			{
				amp = amp / 2; // idk why but the 0Hz amp is too high
			}
			cairo_rectangle(cairoCache, i * ((float) w() / 129), 
					ampHeight - ampHeight * amp, (float) w() / 129,
					ampHeight * amp);
			cairo_fill(cairoCache);
			
			double img = fabs(doubleEmpty[i]) < 0.001 ? 0 : doubleEmpty[i];
			double real = fabs(doubleSmpls[i]) < 0.001 ? 0 : doubleSmpls[i];
			
			float phase = atan2(-real, -img) / M_PI;
			if (phase < 0)
			{
				phase += 2;
			}
			if (amp < 0.001)
			{
				phase = 0;
			}
			cairo_rectangle(cairoCache, i * ((float) w() / 129),
					h() - (h() - ampHeight) * phase * 0.5, (float) w() / 129,
					(h() - ampHeight) * phase * 0.5);
			cairo_fill(cairoCache);
		}
		
		cairo_move_to(cairoCache, 0, ampHeight);
		cairo_line_to(cairoCache, w(), ampHeight);
		theme_->color(cairoCache, Avtk::FG);
		cairo_set_line_width(cairoCache, theme_->lineWidthWide_);
		cairo_stroke(cairoCache);
		
		cairo_restore(cairoCache);
	}
	
	cairo_set_source_surface(cr, surfCache, x(), y());
	//cairo_rectangle(cr, x(), y(), w(), h());
	cairo_paint(cr);
	cairo_rectangle(cr, x(), y(), w(), h());
	theme_->color(cr, Avtk::FG);
	cairo_set_line_width(cairoCache, theme_->lineWidthWide_);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

void HarmonicsView::requestRedraw()
{
	redraw = true;
}

} /* namespace cr42y */
