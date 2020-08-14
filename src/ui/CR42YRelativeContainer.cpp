/*
 * CR42YRelativeContainer.cpp
 *
 *  Created on: 30.07.2020
 *      Author: fabian
 */

#include "CR42YRelativeContainer.h"
#include "CR42YTheme.h"

namespace cr42y
{

CR42YRelativeContainer::CR42YRelativeContainer(CR42YUI* ui) :
		Glib::ObjectBase("CR42YRelativeContainer"),
		Gtk::Container(),
		CR42YWidget(ui)
{
	set_flags(Gtk::NO_WINDOW);
}

CR42YRelativeContainer::~CR42YRelativeContainer()
{

}

void CR42YRelativeContainer::put(Gtk::Widget* child, double x, double y,
		double w, double h, int padLeft, int padTop, int padRight,
		int padBottom)
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
		double w, double h, int padLeft, int padTop, int padRight,
		int padBottom)
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
		if (is_visible() && child->is_visible())
		{
			queue_resize();
		}
	}
}

CR42YRelativeChild* CR42YRelativeContainer::getChild(Gtk::Widget* child)
{
	for (int i = 0; i < children_.size(); i++)
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
	Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
	CR42YTheme* tm = theme();
	float* clr = tm->color(BG_DARK);
	cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
	cr->paint();

	Gtk::Container::on_expose_event(event);
}

void CR42YRelativeContainer::on_size_allocate(Gtk::Allocation& alloc)
{
	Gtk::Allocation childAlloc;
	Gtk::Requisition childReq;

	set_allocation(alloc);

	if (!has_no_window() && is_realized())
	{
		get_window()->move_resize(alloc.get_x(), alloc.get_y(), alloc.get_width(), alloc.get_height());
	}

	for (int i = 0; i < children_.size(); i++)
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
									(int) (get_width() * childData.x)) + childData.padLeft;
		int y = (
				childData.y > 1 ? (int) childData.y :
									(int) (get_height() * childData.y)) + childData.padTop;
		int w = (childData.w > 1 ? (int) childData.w :
									(int) (get_width() * childData.w)) - childData.padLeft - childData.padRight;
		int h = (childData.h > 1 ? (int) childData.h :
									(int) (get_height() * childData.h)) - childData.padTop - childData.padBottom;

		childAlloc.set_x(childAlloc.get_x() + x);
		childAlloc.set_y(childAlloc.get_y() + y);
		childAlloc.set_width(w);
		childAlloc.set_height(h);

		childData.widget->size_allocate(childAlloc);
	}
}

void CR42YRelativeContainer::on_size_request(Gtk::Requisition* requisition)
{

}

void CR42YRelativeContainer::on_remove(Gtk::Widget* child)
{
	CR42YRelativeChild* childData = getChild(child);
	if (childData)
	{
		for (int i = 0; i < children_.size(); i++)
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

void CR42YRelativeContainer::forall_vfunc(gboolean include_internals,
		GtkCallback callback, gpointer callback_data)
{
	for (int i = 0; i < children_.size(); i++)
	{
		if (children_[i].widget->gobj())
		{
			callback(children_[i].widget->gobj(), callback_data);
		}
	}

}

} /* namespace cr42y */
