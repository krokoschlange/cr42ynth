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

#include "CR42YSquareContainer.h"

namespace cr42y
{

CR42YSquareContainer::CR42YSquareContainer() :
		Glib::ObjectBase("CR42YSquareContainer"),
		Gtk::Container(),
		child_(nullptr)
{
	set_flags(Gtk::NO_WINDOW);
}

CR42YSquareContainer::~CR42YSquareContainer()
{
	child_->unparent();
}

void CR42YSquareContainer::on_remove(Gtk::Widget* child)
{
	if (child == child_)
	{
		child_->unparent();
		child_ = nullptr;
	}
}

void CR42YSquareContainer::on_add(Gtk::Widget* child)
{
	child_ = child;
	child_->set_parent(*this);
}

void CR42YSquareContainer::on_realize()
{
	Gtk::Container::on_realize();
	set_realized(true);
}

bool CR42YSquareContainer::on_expose_event(GdkEventExpose* event)
{
	return Gtk::Container::on_expose_event(event);
}

void CR42YSquareContainer::on_size_allocate(Gtk::Allocation& alloc)
{
	set_allocation(alloc);
	
	if (!has_no_window())
	{
		get_window()->move_resize(alloc.get_x(), alloc.get_y(), alloc.get_width(), alloc.get_height());
	}
	
	if (child_)
	{
		Gtk::Allocation childAlloc;
		if (alloc.get_width() > alloc.get_height())
		{
			childAlloc.set_x((alloc.get_width() - alloc.get_height()) / 2);
			childAlloc.set_y(0);
			childAlloc.set_width(alloc.get_height());
			childAlloc.set_height(alloc.get_height());
		}
		else
		{
			childAlloc.set_x(0);
			childAlloc.set_y((alloc.get_height() - alloc.get_width()) / 2);
			childAlloc.set_width(alloc.get_width());
			childAlloc.set_height(alloc.get_width());
		}
		if (has_no_window())
		{
			childAlloc.set_x(childAlloc.get_x() + alloc.get_x());
			childAlloc.set_y(childAlloc.get_y() + alloc.get_y());
		}
		child_->size_allocate(childAlloc);
	}
}

void CR42YSquareContainer::on_size_request(Gtk::Requisition* requisition)
{
	if (child_)
	{
		Gtk::Requisition childReq = child_->size_request();
		if (childReq.width > childReq.height)
		{
			requisition->width = childReq.width;
			requisition->height = childReq.width;
		}
		else
		{
			requisition->width = childReq.height;
			requisition->height = childReq.height;
		}
	}
	else
	{
		requisition->width = 0;
		requisition->height = 0;
	}
}

void CR42YSquareContainer::forall_vfunc(gboolean, GtkCallback callback, gpointer callback_data)
{
	if (child_)
	{
		callback(child_->gobj(), callback_data);
	}
}

}
