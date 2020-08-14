/*
 * CR42YHarmonicsView.cpp
 *
 *  Created on: 03.08.2020
 *      Author: fabian
 */

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
		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius());
		cr->clip_preserve();
		cr->set_line_width(tm->lineThick());
		cr->stroke();

		std::vector<float>* samples = controller_->getSamples(controller_->selectedWaveform());

		std::vector<double> samplesd(samples->begin(), samples->end());
		std::vector<double> empty(samplesd.size());
		delete samples;

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

		float ampHeight = get_height() / 1.5;

		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		for (int i = 0; i < 129 && i < samplesd.size(); i++)
		{
			float amp = sqrt(samplesd[i] * samplesd[i] + empty[i] * empty[i]) / (samplesd.size() / 2) / maxAmp;

			if (i == 0)
			{
				amp = amp / 2; // idk why but the 0Hz amp is too high
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
