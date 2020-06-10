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

#include "theme.hxx"

#include "DragBar.h"

namespace cr42y
{

DragBar::DragBar(Avtk::UI* ui, int x, int y, int w, int h, std::string label,
		bool dS) :
		Avtk::Widget(ui, x, y, w, h, label),
		doubleSided(dS),
		mouseDown(false),
		redraw(true),
		surfCache(nullptr),
		cairoCache(nullptr)
{
	/*surfCache = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	 cairoCache = cairo_create(surfCache);*/

	if (doubleSided)
	{
		defaultValue(0.5);
		value(0.5);
	}
	oldValue = value();
}

DragBar::~DragBar()
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

void DragBar::defaultValue(float dv)
{
	defaultValue_ = dv;
}

float DragBar::defaultValue()
{
	return defaultValue_;
}

void DragBar::draw(cairo_t* cr)
{
	if (redraw || value() != oldValue)
	{
		redraw = false;
		oldValue = value();
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
		
		cairo_rectangle(cairoCache, 0, 0, w(), h());
		theme_->color(cairoCache, Avtk::BG_DARK);
		cairo_fill(cairoCache);
		
		if (doubleSided)
		{
			cairo_move_to(cairoCache, 0, h() / 2);
			cairo_line_to(cairoCache, w(), h() / 2);
			cairo_set_line_width(cairoCache, theme_->lineWidthNorm_);
			theme_->color(cairoCache, Avtk::FG);
			cairo_stroke(cairoCache);
			
			cairo_move_to(cairoCache, w() / 8, h() / 2);
			cairo_line_to(cairoCache, w() / 8, h() * (1 - value()));
			cairo_line_to(cairoCache, w() - w() / 8, h() * (1 - value()));
			cairo_line_to(cairoCache, w() - w() / 8, h() / 2);
			cairo_set_line_width(cairoCache, theme_->lineWidthWide_);
			theme_->color(cairoCache, Avtk::HIGHLIGHT);
			cairo_stroke_preserve(cairoCache);
			
			cairo_close_path(cairoCache);
			theme_->color(cairoCache, Avtk::HIGHLIGHT, 0.2);
			cairo_fill(cairoCache);
		}
		else
		{
			cairo_move_to(cairoCache, 0, h());
			cairo_line_to(cairoCache, w(), h());
			cairo_set_line_width(cairoCache, theme_->lineWidthNorm_);
			theme_->color(cairoCache, Avtk::FG);
			cairo_stroke(cairoCache);
			
			cairo_move_to(cairoCache, w() / 8, h());
			cairo_line_to(cairoCache, w() / 8, h() * (1 - value()));
			cairo_line_to(cairoCache, w() - w() / 8, h() * (1 - value()));
			cairo_line_to(cairoCache, w() - w() / 8, h());
			cairo_set_line_width(cairoCache, theme_->lineWidthWide_);
			theme_->color(cairoCache, Avtk::HIGHLIGHT);
			cairo_stroke_preserve(cairoCache);
			
			cairo_close_path(cairoCache);
			theme_->color(cairoCache, Avtk::HIGHLIGHT, 0.2);
			cairo_fill(cairoCache);
		}
		
		cairo_restore(cairoCache);
	}
	cairo_save(cr);
	
	cairo_set_source_surface(cr, surfCache, x(), y());
	cairo_rectangle(cr, x(), y(), w(), h());
	cairo_fill(cr);
	
	cairo_restore(cr);
}

int DragBar::handle(const PuglEvent* event)
{
	if (!visible())
	{
		return 0;
	}
	if (event->type == PUGL_BUTTON_PRESS && event->button.button == 1 && touches(event->button.x, event->button.y))
	{
		value(1 - (float) (event->button.y - y()) / h());
		callback(this, callbackUD);
		mouseDown = true;
		redraw = true;
		return 1;
	}
	if (event->type == PUGL_BUTTON_PRESS && event->button.button == 3 && touches(event->button.x, event->button.y))
	{
		value(defaultValue());
		callback(this, callbackUD);
		redraw = true;
		return 1;
	}
	if (event->type == PUGL_MOTION_NOTIFY && mouseDown)
	{
		value(1 - (float) (event->button.y - y()) / h());
		callback(this, callbackUD);
		redraw = true;
		return 1;
	}
	if (event->type == PUGL_BUTTON_RELEASE && event->button.button == 1)
	{
		mouseDown = false;
		redraw = true;
	}
	return 0;
}

} /* namespace cr42y */
