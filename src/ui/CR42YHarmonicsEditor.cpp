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

#include "CR42YHarmonicsEditor.h"
#include "WavetableEditController.h"
#include "CR42YBoxVScale.h"
#include "CR42YTheme.h"

#include <iostream>

namespace cr42y
{

CR42YHarmonicsEditor::CR42YHarmonicsEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YHamronicsEditor"),
		CR42YRelativeContainer(ui),
		controller_(nullptr),
		columnWidth_(25)
{
	CR42YTheme* tm = theme();
	int txtHeight = tm->fontSizeSmall() + 2;
	for (int i = 0; i < 128; i++)
	{
		CR42YBoxVScale* scale = new CR42YBoxVScale(ui);
		scale->setDoubleSided(true);
		scale->setValue(0.5, false);
		put(scale, i * columnWidth_, 0, columnWidth_, 0.5);
		scale->signalChanged().connect(sigc::bind<int, bool>(sigc::mem_fun(this, &CR42YHarmonicsEditor::changedCallback), i, true, false));
		scale->signalDone().connect(sigc::bind<int, bool>(sigc::mem_fun(this, &CR42YHarmonicsEditor::changedCallback), i, true, true));
		ampScales_.push_back(scale);

		scale = new CR42YBoxVScale(ui);
		scale->setDoubleSided(true);
		scale->setValue(0.5, false);
		put(scale, i * columnWidth_, 0.5, columnWidth_, 0.5, 0, txtHeight);
		scale->signalChanged().connect(sigc::bind<int, bool>(sigc::mem_fun(this, &CR42YHarmonicsEditor::changedCallback), i, false, false));
		scale->signalDone().connect(sigc::bind<int, bool>(sigc::mem_fun(this, &CR42YHarmonicsEditor::changedCallback), i, false, true));
		phaseScales_.push_back(scale);
	}
	set_size_request(128 * columnWidth_, 10);
}

CR42YHarmonicsEditor::~CR42YHarmonicsEditor()
{
	std::vector<Gtk::Widget*> children = get_children();

	for (int i = 0; i < children.size(); i++)
	{
		remove(*(children[i]));
		delete children[i];
	}
	ampScales_.clear();
	phaseScales_.clear();
}

void CR42YHarmonicsEditor::setController(WavetableEditController* controller)
{
	controller_ = controller;
	update();
}

void CR42YHarmonicsEditor::update()
{
	if (controller_ && controller_->getHarmonicsTable())
	{
		std::vector<std::pair<float, float>>* harmTable = controller_->getHarmonicsTable();
		if (harmTable)
		{
			int max = harmTable->size();
			for (int i = 0; i < max && i < ampScales_.size(); i++)
			{
				ampScales_[i]->set_sensitive(true);
				ampScales_[i]->setValue(((*harmTable)[i].first + 1) / 2, false);
			}
			if (max < ampScales_.size())
			{
				for (int i = max; i < ampScales_.size(); i++)
				{
					ampScales_[i]->set_sensitive(false);
					ampScales_[i]->setValue(0.5, false);
				}
			}
			for (int i = 0; i < max && i < phaseScales_.size(); i++)
			{
				phaseScales_[i]->set_sensitive(true);
				phaseScales_[i]->setValue(((*harmTable)[i].second + 1) / 2, false);
			}
			if (max < phaseScales_.size())
			{
				for (int i = max; i < phaseScales_.size(); i++)
				{
					phaseScales_[i]->set_sensitive(false);
					phaseScales_[i]->setValue(0.5, false);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ampScales_.size(); i++)
		{
			ampScales_[i]->set_sensitive(false);
			ampScales_[i]->setValue(0.5, false);
		}
		for (int i = 0; i < phaseScales_.size(); i++)
		{
			phaseScales_[i]->set_sensitive(false);
			phaseScales_[i]->setValue(0.5, false);
		}
	}
}

bool CR42YHarmonicsEditor::on_expose_event(GdkEventExpose* event)
{
	CR42YRelativeContainer::on_expose_event(event);
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		Cairo::TextExtents xtents;
		std::string str;
		for (int i = 0; i < 128; i++)
		{
			str = i > 0 ? std::to_string(i) : "DC";
			cr->select_font_face(tm->font(), Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
			cr->set_font_size(tm->fontSizeSmall());
			cr->get_text_extents(str, xtents);

			Gtk::Allocation alloc = get_allocation();

			cr->move_to(alloc.get_x() + i * columnWidth_ + (columnWidth_ - xtents.width) / 2, alloc.get_y() + get_height() * 0.5 + (tm->fontSizeSmall() + 2 + xtents.height) / 2);

			cr->show_text(str);
		}
	}
}

void CR42YHarmonicsEditor::changedCallback(double value, int column, bool isAmp, bool end)
{
	if (controller_)
	{
		float amp = isAmp ? value : ampScales_[column]->value();
		float phase = !isAmp ? value : phaseScales_[column]->value();
		amp = amp * 2 - 1;
		phase = phase * 2 - 1;
		controller_->setHarmonic(column, amp, phase, end);
	}
}

} /* namespace cr42y */
