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


#include "CR42YAutomationItem.h"

namespace cr42y
{

CR42YAutomationItem::CR42YAutomationItem(CR42YUI* ui) :
		CR42YToggle(ui),
		generator_(0)
{
	std::vector<Gtk::TargetEntry> entries;
	entries.push_back(Gtk::TargetEntry(("CR42YGenerator"), Gtk::TARGET_SAME_APP, 0));
	drag_source_set(entries, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
}

CR42YAutomationItem::~CR42YAutomationItem()
{

}

void CR42YAutomationItem::setGenerator(uint32_t generator)
{
	generator_ = generator;
	uint8_t type = generator >> 24;
	uint32_t id = generator & 0xffffff;
	std::string text = "";
	if (type == TYPE_ENV)
	{
		text += "ENV";
	}
	else if (type == TYPE_LFO)
	{
		text += "LFO";
	}
	setText(std::to_string(id) + " " + text);
}

void CR42YAutomationItem::on_drag_begin(
	const Glib::RefPtr<Gdk::DragContext>&)
{
}

void CR42YAutomationItem::on_drag_data_get(
	const Glib::RefPtr<Gdk::DragContext>&,
	Gtk::SelectionData& selectionData, guint, guint)
{
	const guchar* data = (const guchar*) &generator_;
	selectionData.set("CR42YGenerator", sizeof(uint32_t) * 8, data, sizeof(uint32_t));
}


void CR42YAutomationItem::on_drag_end(const Glib::RefPtr<Gdk::DragContext>&)
{
}

}
