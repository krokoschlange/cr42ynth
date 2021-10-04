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

#include "CR42YAutomationDial.h"
#include "CR42YTheme.h"
#include "common.h"
#include "helpers.h"

namespace cr42y
{

CR42YAutomationDial::CR42YAutomationDial(CR42YUI* ui) :
	CR42YControlDial(ui)
{
	std::vector<Gtk::TargetEntry> entries;
	entries.push_back(
		Gtk::TargetEntry(("CR42YGenerator"), Gtk::TARGET_SAME_APP,
						 0));
	drag_dest_set(entries, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);
	
	connector_.setWidgetGeneratorSetter((std::function<void(uint32_t)>)std::bind(&CR42YAutomationDial::generatorSetterCallback, this));
}

CR42YAutomationDial::~CR42YAutomationDial()
{

}

void CR42YAutomationDial::generatorSetterCallback()
{
	queue_draw();
	uint32_t generator = connector_.getControl()->getGenerator();
	
	if (generator)
	{
		uint8_t type = generator >> 24;
		uint32_t id = generator & 0xffffff;
		
		std::string tooltip = "";
		if (type == TYPE_ENV || type == TYPE_LFO)
		{
			tooltip += "ENV/LFO ";
		}
		else if (type == TYPE_EXT)
		{
			tooltip += "EXT ";
		}
		tooltip += std::to_string(id);
		set_tooltip_text(tooltip);
		setMode(false);
	}
	else
	{
		set_tooltip_text("");
		setMode(true);
	}
}

bool CR42YAutomationDial::on_expose_event(GdkEventExpose* event)
{
	CR42YControlDial::on_expose_event(event);
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (connector_.getControl() && connector_.getControl()->getGenerator() && win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(HIGHLIGHT);
		
		double size = std::min(0.3 * get_width(), 0.3 * get_height());
		
		cr42y_rounded_rectangle(cr, get_width() - size, 0, size, size, tm->cornerRadius(), tm->lineThick());
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.2);
		cr->fill_preserve();
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke();
	}
	return false;
}

bool CR42YAutomationDial::on_button_press(GdkEventButton* event)
{
	double size = std::min(0.3 * get_width(), 0.3 * get_height());
	if (event->x > get_width() - size && event->y < size)
	{
		connector_.setControlGenerator(0);
		generatorSetterCallback();
		return true;
	}
	return CR42YControlDial::on_button_press(event);
}

void CR42YAutomationDial::on_drag_data_received(
	const Glib::RefPtr<Gdk::DragContext>&, int, int,
	const Gtk::SelectionData& selection_data, guint, guint)
{
	connector_.setControlGenerator(*(uint32_t*) selection_data.get_data());
	generatorSetterCallback();
}

}
