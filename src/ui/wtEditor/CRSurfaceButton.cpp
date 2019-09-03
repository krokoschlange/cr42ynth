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

#include "helpers.hxx"
#include "theme.hxx"

#include "CRSurfaceButton.h"

namespace cr42y
{

CRSurfaceButton::CRSurfaceButton(Avtk::UI* ui, int x, int y, int w,
				int h, std::string label, cairo_surface_t* surf,
				int sw, int sh, cairo_surface_t* pSurf, int psw, int psh) :
		Avtk::Button(ui, x, y, w, h, label),
		surface(nullptr),
		pressedSurface(nullptr),
		scaleX(1),
		scaleY(1),
		pScaleX(1),
		pScaleY(1),
		needsNewSurfaces(true),
		surfInit(surf),
		pSurfInit(pSurf)
{
	label_visible = false;
	scaleX = sw != 0 ? (float) w / sw : 1;
	scaleY = sh != 0 ? (float) h / sh : 1;
	pScaleX = psw != 0 ? (float) w / psw : 1;
	pScaleY = psh != 0 ? (float) h / psh : 1;
	
	/*if (surf)
	{
		surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
		cairo_t* surfCr = cairo_create(surface);
		cairo_scale(surfCr, scaleX, scaleY);
		cairo_set_source_surface(surfCr, surf, 0, 0);
		cairo_paint(surfCr);
		cairo_destroy(surfCr);
	}
	if (pSurf)
	{
		pressedSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
		cairo_t* surfCr = cairo_create(pressedSurface);
		cairo_scale(surfCr, pScaleX, pScaleY);
		cairo_set_source_surface(surfCr, pSurf, 0, 0);
		cairo_paint(surfCr);
		cairo_destroy(surfCr);
	}*/
}

CRSurfaceButton::~CRSurfaceButton()
{
	if (surface != pressedSurface)
	{
		if (surface)
		{
			cairo_surface_destroy(surface);
		}
		if (pressedSurface)
		{
			cairo_surface_destroy(pressedSurface);
		}
	}
	else
	{
		if (surface)
		{
			cairo_surface_destroy(surface);
		}
	}
}

void CRSurfaceButton::draw(cairo_t* cr)
{
	cairo_save(cr);
	
	roundedBox(cr, x(), y(), w(), h(), theme_->cornerRadius_);
	
	if (needsNewSurfaces)
	{
		if (surfInit)
		{
			surface = cairo_surface_create_similar(cairo_get_target(cr),
					CAIRO_CONTENT_COLOR_ALPHA,
					w(), h());
			cairo_t* surfCr = cairo_create(surface);
			cairo_scale(surfCr, scaleX, scaleY);
			cairo_set_source_surface(surfCr, surfInit, 0, 0);
			cairo_paint(surfCr);
			cairo_destroy(surfCr);
		}
		if (pSurfInit)
		{
			pressedSurface = cairo_surface_create_similar(cairo_get_target(cr),
					CAIRO_CONTENT_COLOR_ALPHA,
					w(), h());
			cairo_t* surfCr = cairo_create(pressedSurface);
			cairo_scale(surfCr, scaleX, scaleY);
			cairo_set_source_surface(surfCr, pSurfInit, 0, 0);
			cairo_paint(surfCr);
			cairo_destroy(surfCr);
		}
	}
	
	if (value())
	{
		theme_->color(cr, Avtk::BG_DARK);
		cairo_fill_preserve(cr);
		if (pressedSurface)
		{
			//cairo_translate(cr, x(), y());
			//cairo_scale(cr, pScaleX, pScaleY);
			cairo_set_source_surface(cr, pressedSurface, x(), y());
			cairo_fill_preserve(cr);
		}
		//cairo_identity_matrix(cr);
		theme_->color(cr, Avtk::HIGHLIGHT, 0.2);
		cairo_fill_preserve(cr);
	}
	else
	{
		theme_->color(cr, Avtk::BG_DARK);
		cairo_fill_preserve(cr);
		if (surface)
		{
			//cairo_translate(cr, x(), y());
			//cairo_scale(cr, scaleX, scaleY);
			cairo_set_source_surface(cr, surface, x(), y());
			
			cairo_fill_preserve(cr);
		}
	}
	//cairo_identity_matrix(cr);
	cairo_set_line_width(cr, theme_->lineWidthNorm_);
	theme_->color(cr, Avtk::FG);
	cairo_stroke(cr);
	
	if (label_visible)
	{
		cairo_select_font_face(cr, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size(cr, 10.0);
		cairo_text_extents_t extents;
		cairo_text_extents(cr, label(), &extents);
		cairo_move_to(cr, x() + w() / 2 - extents.width / 2, y() + h());
		if (value())
		{
			theme_->color(cr, Avtk::BG_DARK);
		}
		else
		{
			theme_->color(cr, Avtk::FG);
		}
		cairo_show_text(cr, label());
	}
	
	cairo_restore(cr);
}


} /* namespace cr42y */
