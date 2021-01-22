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
#include "CR42YWFView.h"

#include "CR42YButton.h"
#include "CR42YTheme.h"
#include "CR42YToggleSelector.h"
#include "WavetableEditController.h"
#include "WavetableEditData.h"

namespace cr42y
{

CR42YWFView::CR42YWFView(CR42YUI* ui, WavetableEditController* controller, CR42YToggleSelector* viewSelector) :
		Glib::ObjectBase("CR42YWFView"),
		CR42YGrid(ui),
		showAll_(false),
		selectedWaveform_(0),
		viewSelector_(viewSelector),
		data_(nullptr),
		controller_(controller),
		editBtn_(new CR42YButton(ui))
{
	setDrawBG(false);
	setDrawBorder(true);

	configureColumn(0, 10, 0, 0, 0, 0);
	configureColumn(1, 1, 0.003, 0.003, 0, 0);

	configureRow(0, 1, 0.003, 0.003, 0, 0);
	configureRow(1, 7, 0, 0, 0, 0);


	editBtn_->setText("Edit");
	editBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YWFView::editWFCallback));

	put(editBtn_, 0, 1);
}

CR42YWFView::~CR42YWFView()
{
}

void CR42YWFView::setShowAll(bool showAll)
{
	showAll_ = showAll;
}

void CR42YWFView::setSelectedWaveform(float selectedWaveform)
{
	if (data_)
	{
		selectedWaveform_ = selectedWaveform * (data_->getWaveforms()->size() - 1);
	}
	else
	{
		selectedWaveform_ = 0;
	}
	queue_draw();
}

void CR42YWFView::setData(WavetableEditData* data)
{
	data_ = data;
	queue_draw();
}

void CR42YWFView::on_realize()
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

		window_ = Gdk::Window::create(get_window(), &attributes,
				GDK_WA_X | GDK_WA_Y);

		unset_flags(Gtk::NO_WINDOW);
		set_window(window_);

		window_->set_user_data(gobj());
	}
}

bool CR42YWFView::on_expose_event(GdkEventExpose* event)
{
	if (data_)
	{
		Glib::RefPtr<Gdk::Window> win = get_window();
		if (win)
		{
			Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();

			if (showAll_)
			{
				//TODO: fake 3D view
			}
			else
			{
				CR42YTheme* tm = theme();
				float* clr = tm->color(BG_DARK);

				cr->rectangle(0, 0, get_width(), get_height());
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

				cr->fill();

				int stepSize = data_->getWidth() / get_width();
				if (stepSize < 1)
				{
					stepSize = 1;
				}
				std::vector<float> samples;
				data_->getSamples(samples, selectedWaveform_, stepSize);

				float pixelsPerSample = (float) get_width() / samples.size();


				for (size_t i = 1; i < samples.size(); i++)
				{
					cr->move_to(i * pixelsPerSample, (samples[i] * get_height() - get_height()) / -2);
					cr->line_to((i - 1) * pixelsPerSample, (samples[i - 1] * get_height() - get_height()) / -2);
				}

				cr->move_to(get_width(), (samples[0] * get_height() - get_height()) / -2);
				cr->line_to(get_width() - pixelsPerSample, (samples[samples.size() - 1] * get_height() - get_height()) / -2);

				clr = tm->color(HIGHLIGHT);

				cr->set_line_width(tm->lineThick());
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
				cr->stroke();
			}
		}
	}

	return CR42YGrid::on_expose_event(event);
}


void CR42YWFView::editWFCallback()
{
	if (data_)
	{
		controller_->setData(data_, true);
		viewSelector_->select(3);
	}
}

} /* namespace cr42y */
