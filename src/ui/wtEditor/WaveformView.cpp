/*
 * WaveformView.cpp
 *
 *  Created on: 12.08.2019
 *      Author: fabian
 */

#include <iostream>

#include "WaveformView.h"

namespace cr42y
{

WaveformView::WaveformView(WTEditor* ed, Avtk::UI* ui, int x, int y, int w, int h, std::string label) :
		Avtk::Widget(ui, x, y, w, h, label),
		editor(ed),
		redraw(true),
		surfCache(nullptr),
		cairoCache(nullptr)
{
	surfCache = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	cairoCache = cairo_create(surfCache);
	std::cout << "kk";
}

WaveformView::~WaveformView()
{
	cairo_surface_destroy(surfCache);
	cairo_destroy(cairoCache);
}

void WaveformView::draw(cairo_t* cr)
{
	cairo_save(cr);
	std::cout << "REDRAW";

	if (redraw)
	{
		cairo_rectangle(cairoCache, 0, 0, w(), h());
		theme_->color(cairoCache, Avtk::BG_DARK);
		cairo_fill(cairoCache);

		std::vector<float>* samples = nullptr;
		if (editor->getEditData())
		{
			samples = editor->getEditData()->getSamples(editor->getWTPos());
		}
		else
		{
			samples = new std::vector<float>();
			samples->push_back(0);
			samples->push_back(0);
		}

		float pixelPerSample = w() / (samples->size() - 1);

		for (int i = 1; i < samples->size(); i++)
		{
			cairo_move_to(cairoCache, pixelPerSample * i, h() * -1 * (*samples)[i] + h() / 2);
			cairo_line_to(cairoCache, pixelPerSample * (i - 1), h() * -1 * (*samples)[i - 1] + h() / 2);
		}
		cairo_set_line_join(cairoCache, CAIRO_LINE_JOIN_ROUND);
		cairo_set_line_width(cairoCache, theme_->lineWidthThin());
		theme_->color(cairoCache, Avtk::FG);

		cairo_stroke(cairoCache);

	}

	cairo_set_source_surface(cr, surfCache, x(), y());
	cairo_rectangle(cr, x(), y(), w(), h());
	cairo_fill(cr);

	theme_->color(cr, Avtk::FG);
	cairo_stroke(cr);

	cairo_restore(cr);
}

} /* namespace cr42y */
