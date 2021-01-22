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

#include "CR42YRelativeContainer.h"
#include "CR42YTheme.h"
#include "helpers.h"

namespace cr42y
{

CR42YRelativeContainer::CR42YRelativeContainer(CR42YUI* ui) :
		Glib::ObjectBase("CR42YRelativeContainer"),
		Gtk::Container(),
		CR42YWidget(ui),
		drawBorder_(false),
		drawBG_(true)
{
	set_flags(Gtk::NO_WINDOW);
}

CR42YRelativeContainer::~CR42YRelativeContainer()
{

}

void CR42YRelativeContainer::put(Gtk::Widget* child, double x, double y,
		double w, double h, double padLeft, double padTop, double padRight,
		double padBottom)
{
	CR42YRelativeChild childData;
	childData.widget = child;
	childData.x = x;
	childData.y = y;
	childData.w = w;
	childData.h = h;
	childData.padLeft = padLeft;
	childData.padTop = padTop;
	childData.padRight = padRight;
	childData.padBottom = padBottom;
	children_.push_back(childData);
	child->set_parent(*this);
	//add(*child);
}

void CR42YRelativeContainer::move(Gtk::Widget* child, double x, double y,
		double w, double h, double padLeft, double padTop, double padRight,
		double padBottom)
{
	CR42YRelativeChild* childData = getChild(child);
	if (childData)
	{
		childData->x = x;
		childData->y = y;
		childData->w = w;
		childData->h = h;
		childData->padLeft = padLeft;
		childData->padTop = padTop;
		childData->padRight = padRight;
		childData->padBottom = padBottom;
		/*if (is_visible() && child->is_visible())
		{
			queue_resize();
		}*/
	}
}

CR42YRelativeChild* CR42YRelativeContainer::getChild(Gtk::Widget* child)
{
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (children_[i].widget == child)
		{
			return &(children_[i]);
		}
	}
	return nullptr;
}

void CR42YRelativeContainer::on_realize()
{
	Gtk::Container::on_realize();
	set_realized(true);
}

bool CR42YRelativeContainer::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		if (drawBG_)
		{
			cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->fill();
		}

		if (drawBorder_)
		{
			clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());

			if (has_no_window())
			{
				cr42y_rounded_rectangle(cr, get_allocation().get_x(), get_allocation().get_y(), get_width(), get_height(), tm->cornerRadius(), tm->lineThick());
			}
			else
			{
				cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius(), tm->lineThick());
			}
			cr->stroke();
		}
	}

	return Gtk::Container::on_expose_event(event);
}

void CR42YRelativeContainer::on_size_allocate(Gtk::Allocation& alloc)
{
	Gtk::Allocation childAlloc;

	set_allocation(alloc);

	if (!has_no_window())
	{
		get_window()->move_resize(alloc.get_x(), alloc.get_y(), alloc.get_width(), alloc.get_height());
	}

	for (size_t i = 0; i < children_.size(); i++)
	{
		CR42YRelativeChild childData = children_[i];
		childAlloc.set_x(0);
		childAlloc.set_y(0);
		if (has_no_window())
		{
			childAlloc.set_x(get_allocation().get_x());
			childAlloc.set_y(get_allocation().get_y());
		}

		int x = (
				childData.x > 1 ? (int) childData.x :
									(int) std::round(get_width() * childData.x)); // + childData.padLeft;
		int y = (
				childData.y > 1 ? (int) childData.y :
									(int) std::round(get_height() * childData.y)); // + childData.padTop;
		int w = (
				childData.w > 1 ? (int) childData.w :
									(int) std::round(get_width() * childData.w)); // - childData.padLeft - childData.padRight;
		int h = (
				childData.h > 1 ? (int) childData.h :
									(int) std::round(get_height() * childData.h)); // - childData.padTop - childData.padBottom;

		if (fabs(childData.padLeft) < 1)
		{
			x += std::round(childData.padLeft * get_toplevel()->get_height());
			w -= std::round(childData.padLeft * get_toplevel()->get_height());
		}
		else
		{
			x += childData.padLeft;
			w -= childData.padLeft;
		}
		w -= fabs(childData.padRight) > 1 ?
				childData.padRight :
				std::round(childData.padRight * get_toplevel()->get_height());
		if (fabs(childData.padTop) < 1)
		{
			y += std::round(childData.padTop * get_toplevel()->get_height());
			h -= std::round(childData.padTop * get_toplevel()->get_height());
		}
		else
		{
			y += childData.padTop;
			h -= childData.padTop;
		}
		h -= fabs(childData.padBottom) > 1 ?
				childData.padBottom :
				std::round(childData.padBottom * get_toplevel()->get_height());

		childAlloc.set_x(childAlloc.get_x() + x);
		childAlloc.set_y(childAlloc.get_y() + y);
		childAlloc.set_width(w);
		childAlloc.set_height(h);

		childData.widget->size_allocate(childAlloc);
	}
}

void CR42YRelativeContainer::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();
	requisition->width = 0;
	requisition->height = 0;
}

void CR42YRelativeContainer::on_remove(Gtk::Widget* child)
{
	CR42YRelativeChild* childData = getChild(child);
	if (childData)
	{
		for (size_t i = 0; i < children_.size(); i++)
		{
			if (&(children_[i]) == childData)
			{
				children_.erase(children_.begin() + i);
				child->unparent();
				i--;
			}
		}
	}
}

void CR42YRelativeContainer::on_add(Gtk::Widget* child)
{
	put(child, 0, 0, 1, 1);
}

void CR42YRelativeContainer::forall_vfunc(gboolean,
		GtkCallback callback, gpointer callback_data)
{
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (children_[i].widget->gobj())
		{
			callback(children_[i].widget->gobj(), callback_data);
		}
	}

}

void CR42YRelativeContainer::setDrawBorder(bool drawBorder)
{
	drawBorder_ = drawBorder;
}

bool CR42YRelativeContainer::drawBorder()
{
	return drawBorder_;
}

void CR42YRelativeContainer::setDrawBG(bool drawBG)
{
	drawBG_ = drawBG;
}

bool CR42YRelativeContainer::drawBG()
{
	return drawBG_;
}

} /* namespace cr42y */
