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
/*
 * CR42YWavetableView.cpp
 *
 *  Created on: 07.08.2020
 *      Author: fabian
 */

#include "CR42YWavetableView.h"
#include "CR42YTheme.h"
#include "helpers.h"
#include "WavetableEditController.h"
#include "CR42YButton.h"
#include "CR42YWavetableViewItem.h"

namespace cr42y
{

CR42YWavetableView::CR42YWavetableView(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWavetableView"),
		CR42YRelativeContainer(ui),
		controller_(nullptr),
		boxSize_(50)
{
	setDrawBorder(true);

	set_flags(Gtk::NO_WINDOW);

	std::vector<Gtk::TargetEntry> entries;
	entries.push_back(Gtk::TargetEntry(("CR42YWavtableViewItem"), Gtk::TARGET_SAME_APP, 0));
	drag_dest_set(entries, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);
	/*add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
	 signal_button_press_event().connect(sigc::mem_fun(this, &CR42YWavetableView::on_button_press_event));
	 signal_motion_notify_event().connect(sigc::mem_fun(this, &CR42YWavetableView::on_motion_notify_event));

	 signal_button_release_event().connect(sigc::mem_fun(this, &CR42YWavetableView::on_button_release_event));

	 std::vector<Gtk::TargetEntry> entries;
	 entries.push_back(Gtk::TargetEntry("CR42YWavetableView", Gtk::TARGET_SAME_WIDGET, 0));
	 drag_source_set(entries, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	 drag_dest_set(entries, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);

	 signal_drag_data_get().connect(sigc::mem_fun(this, &CR42YWavetableView::on_drag_data_get));*/

}

CR42YWavetableView::~CR42YWavetableView()
{
}

void CR42YWavetableView::setController(WavetableEditController* controller)
{
	controller_ = controller;
	update();
}

void CR42YWavetableView::on_realize()
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
	/* update or the first (and only) CR42YWavetableViewItem will show a remove
	 * Button, which we dont want
	 */
	update();
}

void CR42YWavetableView::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();
	if (controller_)
	{
		int height = boxSize_ * controller_->getWavetableHeight();
		requisition->height = std::max<int>(height, get_parent()->get_height());
		requisition->width = 10;
	}
	else
	{
		requisition->height = get_parent()->get_height();
		requisition->width = 10;
	}
}

void CR42YWavetableView::update()
{
	if (controller_)
	{
		for (int i = items_.size(); i < controller_->getWavetableHeight(); i++)
		{
			CR42YWavetableViewItem* item = new CR42YWavetableViewItem(ui_);
			item->setController(controller_);
			item->setWaveform(i);
			item->setShowButton(true);

			put(item, 0, boxSize_ * i, 1, boxSize_);
			items_.push_back(item);
			item->show();
		}
		while (controller_->getWavetableHeight() < items_.size())
		{
			remove(*(items_[items_.size() - 1]));
			delete items_[items_.size() - 1];
			items_.erase(items_.begin() + items_.size() - 1);
		}
		for (int i = 0; i < items_.size(); i++)
		{
			items_[i]->queue_draw();
		}

		if (get_parent())
		{
			set_size_request(10, std::max<int>(boxSize_ * controller_->getWavetableHeight(), get_parent()->get_height()));
		}
		else
		{
			set_size_request(10, boxSize_ * controller_->getWavetableHeight());
		}
	}
}

void CR42YWavetableView::on_drag_data_received(
		const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
		const Gtk::SelectionData& selection_data, guint info, guint time)
{
	CR42YWavetableViewItem* source = *((CR42YWavetableViewItem**) selection_data.get_data());

	bool isSourceSelected = controller_->selectedWaveform() == source->waveform();

	controller_->moveWaveform(source->waveform(), controller_->getWavetableHeight());

	if (isSourceSelected)
	{
		int selectPos = controller_->getWavetableHeight();
		if (controller_->getWavetableHeight() > source->waveform())
		{
			selectPos--;
		}
		controller_->selectWaveform(selectPos);
	}
	else if (controller_->getWavetableHeight() > controller_->selectedWaveform() && source->waveform() <= controller_->selectedWaveform())
	{
		controller_->selectWaveform(controller_->selectedWaveform() - 1);
	}
}

}
/* namespace cr42y */
