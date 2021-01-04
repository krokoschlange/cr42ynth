/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
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

#include "CR42YHarmonicsView.h"
#include "CR42YTheme.h"
#include "WavetableEditController.h"
#include "FftRealPair.hpp"
#include "helpers.h"

namespace cr42y
{

CR42YHarmonicsView::CR42YHarmonicsView(CR42YUI* ui) :
		Glib::ObjectBase("CR42YHarmonicsView"),
		Gtk::Widget(),
		CR42YWidget(ui),
		controller_(nullptr)
{
	set_flags(Gtk::NO_WINDOW);
}

CR42YHarmonicsView::~CR42YHarmonicsView()
{
}

void CR42YHarmonicsView::setController(WavetableEditController* controller)
{
	controller_ = controller;
}

bool CR42YHarmonicsView::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win && controller_)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		cr->paint();
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius(), tm->lineThick());
		cr->set_line_width(tm->lineThick());
		cr->stroke_preserve();
		cr->clip();


		std::vector<float> samples;
		controller_->getSamples(samples, controller_->selectedWaveform());

		std::vector<double> samplesd(samples.begin(), samples.end());
		std::vector<double> empty(samplesd.size());

		Fft::transform(samplesd, empty);

		float maxAmp = 0;
		for (int i = 0; i < 129 && i < samplesd.size(); i++)
		{
			float amp = sqrt(samplesd[i] * samplesd[i] + empty[i] * empty[i]) / (samplesd.size() / 2);
			if (amp > maxAmp)
			{
				maxAmp = amp;
			}
		}
		if (fabsf(maxAmp) < 0.001)
		{
			maxAmp = 1;
		}

		float ampHeight = get_height() / 1.5;

		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		for (int i = 0; i < 129 && i < samplesd.size(); i++)
		{
			float amp = sqrt(samplesd[i] * samplesd[i] + empty[i] * empty[i]) / (samplesd.size() / 2) / maxAmp;

			if (i == 0)
			{
				amp = amp / 2;
			}
			cr->rectangle(i * ((float) get_width() / 129), ampHeight - ampHeight * amp, (float) get_width() / 129, ampHeight * amp);
			cr->fill();

			double img = fabs(empty[i]) < 0.001 ? 0 : empty[i];
			double real = fabs(samplesd[i]) < 0.001 ? 0 : samplesd[i];

			float phase = atan2(-real, -img) / M_PI;
			if (phase < 0)
			{
				phase += 2;
			}
			if (amp < 0.001)
			{
				phase = 0;
			}
			cr->rectangle(i * ((float) get_width() / 129), get_height() - (get_height() - ampHeight) * phase * 0.5, (float) get_width() / 129, (get_height() - ampHeight) * phase * 0.5);
			cr->fill();
		}

		cr->move_to(0, ampHeight);
		cr->line_to(get_width(), ampHeight);
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke();
	}
}

void CR42YHarmonicsView::on_realize()
{
	Gtk::Widget::on_realize();

	if (!window_)
	{
		GdkWindowAttr attributes;
		memset(&attributes, 0, sizeof(attributes));

		Gtk::Allocation alloc = get_allocation();

		attributes.x = alloc.get_x();
		attributes.y = alloc.get_y();
		attributes.width = alloc.get_width();
		attributes.height = alloc.get_height();

		attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;

		window_ = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);

		unset_flags(Gtk::NO_WINDOW);
		set_window(window_);

		window_->set_user_data(gobj());
	}
}

void CR42YHarmonicsView::on_size_allocate(Gtk::Allocation& alloc)
{
	set_allocation(alloc);
	if (window_)
	{
		window_->move_resize(alloc.get_x(), alloc.get_y(), alloc.get_width(), alloc.get_height());
	}
}

} /* namespace cr42y */
