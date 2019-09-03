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

#include "slider.hxx"
#include "theme.hxx"
#include "ui.hxx"

#include "CustomScroll.h"

namespace cr42y
{

CustomScroll::CustomScroll(Avtk::UI* ui, int x, int y, int w, int h, std::string label) :
		Avtk::Group(ui, x, y, w, h, label),
		child(nullptr),
		doScrollX(false),
		doScrollY(false),
		grab(GRAB_NONE),
		scrollX(0),
		scrollY(0),
		scrollXAmount(0),
		scrollYAmount(0),
		maxScrollX(0),
		maxScrollY(0),
		scrollXRange(0),
		scrollYRange(0),
		scrollXSize(0),
		scrollYSize(0),
		cairoCache(nullptr),
		surfCache(nullptr),
		needsNewChildCr(true)
{
}

CustomScroll::~CustomScroll()
{
	
}

void CustomScroll::draw(cairo_t* cr)
{
	cairo_save(cr);
	
	cairo_rectangle(cr, x(), y(), w(), h());
	theme_->color(cr, Avtk::BG_DARK);
	cairo_fill(cr);
	if (child)
	{
		if (!surfCache || needsNewChildCr)
		{
			needsNewChildCr = false;
			if (surfCache)
			{
				cairo_surface_destroy(surfCache);
			}
			if (cairoCache)
			{
				cairo_destroy(cairoCache);
			}
			surfCache = cairo_surface_create_similar(
					cairo_get_target(cr),
					CAIRO_CONTENT_COLOR_ALPHA,
					child->w(), child->h());
			cairoCache = cairo_create(surfCache);
		}
		//cairo_rectangle(cairoCache, 0, 0, child->w(), child->h());
		theme_->color(cairoCache, Avtk::FG);
		cairo_paint(cairoCache);
		child->draw(cairoCache);
		
		cairo_rectangle(cr, x(), y(), w(), h());
		cairo_set_source_surface(cr, surfCache, x() - scrollXAmount, y() - scrollYAmount);
		cairo_fill(cr);
		
		if (doScrollX)
		{
			cairo_rectangle(cr, x(), y() + h() - 10, w(), 10);
			theme_->color(cr, Avtk::BG);
			cairo_fill(cr);
			cairo_rectangle(cr, x() + scrollX * (scrollXRange - scrollXSize),
					y() + h() - 10, scrollXSize, 10);
			theme_->color(cr, Avtk::FG);
			cairo_fill(cr);
		}
		if (doScrollY)
		{
			cairo_rectangle(cr, x() + w() - 10, y(), 10, h());
			theme_->color(cr, Avtk::BG);
			cairo_fill(cr);
			cairo_rectangle(cr, x() + w() - 10,
					y() + scrollY * (scrollYRange - scrollYSize), 10, scrollYSize);
			theme_->color(cr, Avtk::FG);
			cairo_fill(cr);
		}
	}
	cairo_rectangle(cr, x(), y(), w(), h());
	theme_->color(cr, Avtk::FG);
	cairo_set_line_width(cr, theme_->lineWidthNorm_);
	cairo_stroke(cr);
	
	cairo_restore(cr);
}

int CustomScroll::handle(const PuglEvent* event)
{
	if (event->type == PUGL_BUTTON_PRESS && event->button.button == 1)
	{
		if (touches(event->button.x, event->button.y))
		{
			if (doScrollY && event->button.x > x() + w() - 10 &&
					event->button.x < x() + w())
			{
				int deltaY = event->button.y - y();
				if (deltaY <= scrollYRange && deltaY >= 0)
				{
					grab = GRAB_VERTICAL;
					mx = event->button.x;
					my = event->button.y;
					if (deltaY < scrollY * (scrollYRange - scrollYSize) ||
							deltaY > scrollY * (scrollYRange - scrollYSize) + scrollYSize)
					{
						scrollY = (float) (deltaY - scrollYSize / 2) / (scrollYRange - scrollYSize);
					}
					scrollY = scrollY < 0 ? 0 : scrollY;
					scrollY = scrollY > 1 ? 1 : scrollY;
					scrollYAmount = scrollY * maxScrollY;
					ui->redraw(this);
					return 1;
				}
			}
			if (doScrollX && event->button.y > y() + h() - 10 &&
					event->button.y < y() + h())
			{
				int deltaX = event->button.x - x();
				if (deltaX <= scrollXRange && deltaX >= 0)
				{
					grab = GRAB_HORIZONTAL;
					mx = event->button.x;
					my = event->button.y;
					if (deltaX < scrollX * (scrollXRange - scrollXSize) ||
							deltaX > scrollX * (scrollXRange - scrollXSize) + scrollXSize)
					{
						scrollX = (float) (deltaX - scrollXSize / 2) / (scrollXRange - scrollXSize);
					}
					scrollX = scrollX < 0 ? 0 : scrollX;
					scrollX = scrollX > 1 ? 1 : scrollX;
					scrollXAmount = scrollX * maxScrollX;
					ui->redraw(this);
					return 1;
				}
			}
		}
	}
	if (event->type == PUGL_MOTION_NOTIFY)
	{
		if (grab == GRAB_VERTICAL)
		{
			int deltaY = event->motion.y - my;
			int sliderYPos = scrollY * (scrollYRange - scrollYSize) + deltaY;
			scrollY = (float) sliderYPos / (scrollYRange - scrollYSize);
			scrollY = scrollY < 0 ? 0 : scrollY;
			scrollY = scrollY > 1 ? 1 : scrollY;
			scrollYAmount = scrollY * maxScrollY;
			ui->redraw(this);
			my = event->motion.y;
			return 1;
		}
		if (grab == GRAB_HORIZONTAL)
		{
			int deltaX = event->motion.x - mx;
			int sliderXPos = scrollX * (scrollXRange - scrollXSize) + deltaX;
			scrollX = (float) sliderXPos / (scrollXRange - scrollXSize);
			scrollX = scrollX < 0 ? 0 : scrollX;
			scrollX = scrollX > 1 ? 1 : scrollX;
			scrollXAmount = scrollX * maxScrollX;
			ui->redraw(this);
			mx = event->motion.x;
			return 1;
		}
	}
	if (event->type == PUGL_SCROLL)
	{
		if (touches(event->scroll.x, event->scroll.y))
		{
			int ret = 0;
			if (doScrollY)
			{
				scrollYAmount -= 8 * event->scroll.dy;
				scrollYAmount = scrollYAmount < 0 ? 0 : scrollYAmount;
				scrollYAmount = scrollYAmount > maxScrollY ? maxScrollY :
						scrollYAmount;
				scrollY = (float) scrollYAmount / maxScrollY;
				ui->redraw(this);
				ret = 1;
			}
			if (doScrollX)
			{
				scrollXAmount += 8 * event->scroll.dx;
				scrollXAmount = scrollXAmount < 0 ? 0 : scrollXAmount;
				scrollXAmount = scrollXAmount > maxScrollX ? maxScrollX :
						scrollXAmount;
				scrollX = (float) scrollXAmount / maxScrollX;
				ui->redraw(this);
				ret = 1;
			}
		}
	}
	if (event->type == PUGL_BUTTON_RELEASE && event->button.button == 1)
	{
		grab = GRAB_NONE;
	}
	if (((event->type == PUGL_BUTTON_PRESS/* || event->type == PUGL_BUTTON_RELEASE*/) &&
			touches(event->button.x, event->button.y)) ||
			(event->type == PUGL_SCROLL && touches(event->scroll.x, event->scroll.y)) ||
			/*(event->type == PUGL_MOTION_NOTIFY &&
			touches(event->motion.x, event->motion.y)) ||*/
			(event->type != PUGL_BUTTON_PRESS/* && event->type != PUGL_BUTTON_RELEASE*/ &&
			event->type != PUGL_SCROLL/* && event->type != PUGL_MOTION_NOTIFY*/))
	{
		PuglEvent childEvent;
		offsetEvent(event, &childEvent);
		return child->handle(&childEvent);
	}
	return 0;
}

void CustomScroll::setChild(Avtk::Widget* c)
{
	if (c->parent())
	{
		c->parent()->remove(child);
	}
	if (child)
	{
		remove(child);
	}
	add(c);
	child = c;
	
	child->x(0);
	child->y(0);
	
	/*if (child->w() > w() || (child->w() > w() - 10 && child->h() > h()))
	{
		doScrollX = true;
		maxScrollX = child->w() - w();
		if (child->h() > h())
		{
			maxScrollX += 10;
		}
	}
	if (child->h() > h() || (child->h() > h() - 10 && child->w()))*/
	childResize();
	scrollX = 0;
	scrollXAmount = 0;
	scrollY = 0;
	scrollYAmount = 0;
}

void CustomScroll::childResize()
{
	if (child->w() > w())
	{
		doScrollX = true;
	}
	else
	{
		doScrollX = false;
	}
	if (child->h() > h())
	{
		doScrollY = true;
	}
	else
	{
		doScrollY = false;
	}
	if (doScrollY && child->w() > w() - 10)
	{
		doScrollX = true;
	}
	if (doScrollX && child->h() > h() - 10)
	{
		doScrollY = true;
	}
	if (doScrollX)
	{
		if (doScrollY)
		{
			maxScrollX = child->w() - w() - 10;
			scrollXSize = (w() - 10) * w() / child->w();
			scrollXRange = w() - 10;
		}
		else
		{
			maxScrollX = child->w() - w();
			scrollXSize = w() * w() / child->w();
			scrollXRange = w();
		}
	}
	if (doScrollY)
	{
		if (doScrollX)
		{
			maxScrollY = child->h() - h() - 10;
			scrollYSize = (h() - 10) * h() / child->h();
			scrollYRange = h() - 10;
		}
		else
		{
			maxScrollY = child->h() - h();
			scrollYSize = h() * h() / child->h();
			scrollYRange = h();
		}
	}
	if (surfCache)
	{
		//cairo_surface_destroy(surfCache);
		//surfCache = nullptr;
		needsNewChildCr = true;
	}
	ui->redraw(this);
}

void CustomScroll::offsetEvent(const PuglEvent* in, PuglEvent* out)
{
	*out = *in;
	if (out->type == PUGL_BUTTON_PRESS || out->type == PUGL_BUTTON_RELEASE)
	{
		out->button.x -= x() - scrollXAmount;
		out->button.y -= y() - scrollYAmount;
	}
	else if (out->type == PUGL_SCROLL)
	{
		out->scroll.x -= x() - scrollXAmount;
		out->scroll.y -= y() - scrollYAmount;
	}
	else if (out->type == PUGL_MOTION_NOTIFY)
	{
		out->motion.x -= x() - scrollXAmount;
		out->motion.y -= y() - scrollYAmount;
	}
}

} /* namespace cr42y */
