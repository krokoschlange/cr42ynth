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
}

void CR42YWavetableView::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();
	if (controller_)
	{
		requisition->height = boxSize_ * controller_->getWavetableHeight();
		requisition->width = 10;
	}
	else
	{
		requisition->height = 10;
		requisition->width = 10;
	}
}

void CR42YWavetableView::update()
{
	std::vector<Gtk::Widget*> children = get_children();

	for (int i = 0; i < children.size(); i++)
	{
		Gtk::Widget* child = children[i];
		remove(*child);
		delete child;
	}

	if (controller_)
	{
		bool removeBtns = controller_->getWavetableHeight() > 1;
		for (int i = 0; i < controller_->getWavetableHeight(); i++)
		{
			CR42YWavetableViewItem* item = new CR42YWavetableViewItem(ui_, removeBtns);
			item->setController(controller_);
			item->setWaveform(i);

			if (removeBtns)
			{
				item->removeButtonClickedSignal().connect(sigc::bind<int>(sigc::mem_fun(this, &CR42YWavetableView::removeWaveformCallback), i));
			}
			item->waveformSelectedSignal().connect(sigc::bind<int>(sigc::mem_fun(this, &CR42YWavetableView::selectWaveformCallback), i));

			put(item, 0, boxSize_ * i, 1, boxSize_);
			item->show();
		}
	}
}

sigc::signal<void> CR42YWavetableView::waveformRemovedSignal()
{
	return waveformRemovedSignal_;
}

void CR42YWavetableView::removeWaveformCallback(int waveform)
{
	if (controller_)
	{
		controller_->removeWaveform(waveform);
		update();
		waveformRemovedSignal_.emit();
	}
}

void CR42YWavetableView::selectWaveformCallback(int waveform)
{
	if (controller_)
	{
		controller_->selectWaveform(waveform);
		queue_draw();
		waveformRemovedSignal_.emit();
	}
}

void CR42YWavetableView::on_drag_data_received(
		const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
		const Gtk::SelectionData& selection_data, guint info, guint time)
{
	CR42YWavetableViewItem* source = *((CR42YWavetableViewItem**) selection_data.get_data());

	controller_->moveWaveform(source->waveform(), -1);
	selectWaveformCallback(controller_->getWavetableHeight() - 1);
}

}
/* namespace cr42y */
