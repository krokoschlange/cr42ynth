/*
 * CR42YWavetableViewItem.cpp
 *
 *  Created on: 09.08.2020
 *      Author: fabian
 */

#include "CR42YWavetableViewItem.h"
#include "CR42YButton.h"
#include "CR42YTheme.h"
#include "helpers.h"
#include "WavetableEditController.h"
#include "CR42YUI.h"

namespace cr42y
{

CR42YWavetableViewItem::CR42YWavetableViewItem(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWavetableViewItem"),
		CR42YRelativeContainer(ui),
		removeBtn_(nullptr),
		waveform_(0),
		controller_(nullptr),
		dropLocation_(-1),
		mouseDown_(false),
		inDnD_(false)
{
	set_flags(Gtk::NO_WINDOW);

	//add_events(Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YWavetableViewItem::custom_button_press_event));
	signal_button_release_event().connect(sigc::mem_fun(this, &CR42YWavetableViewItem::custom_button_release_event));

	std::vector<Gtk::TargetEntry> entries;
	entries.push_back(Gtk::TargetEntry(("CR42YWavtableViewItem"), Gtk::TARGET_SAME_APP, 0));
	drag_source_set(entries, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	drag_dest_set(entries, Gtk::DEST_DEFAULT_ALL, Gdk::ACTION_MOVE);

	removeBtn_ = new CR42YButton(ui);
	Cairo::RefPtr<Cairo::Surface> min = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/minus.png");
	removeBtn_->setSurfActive(min);
	removeBtn_->setSurfInactive(min);
	removeBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YWavetableViewItem::removeBtnClicked));

	put(removeBtn_, 0, 0, 25, 25, 5, 5);
}

CR42YWavetableViewItem::~CR42YWavetableViewItem()
{
	if (removeBtn_)
	{
		delete removeBtn_;
	}
}

void CR42YWavetableViewItem::setController(WavetableEditController* controller)
{
	controller_ = controller;
}

void CR42YWavetableViewItem::setWaveform(int waveform)
{
	waveform_ = waveform;
}

int CR42YWavetableViewItem::waveform()
{
	return waveform_;
}

void CR42YWavetableViewItem::setShowButton(bool show)
{
	if (removeBtn_)
	{
		if (show)
		{
			removeBtn_->show();
		}
		else
		{
			removeBtn_->hide();
		}
	}
}

void CR42YWavetableViewItem::on_realize()
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

bool CR42YWavetableViewItem::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		draw(cr);
	}
	Gtk::Container::on_expose_event(event);
}

void CR42YWavetableViewItem::draw(Cairo::RefPtr<Cairo::Context> cr)
{
	CR42YTheme* tm = theme();
	float* clr = tm->color(BG_DARK);

	cr->rectangle(0, 0, get_width(), get_height());
	cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
	cr->fill();

	cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius()), tm->lineThick();

	if (controller_)
	{
		if (controller_->selectedWaveform() == waveform_)
		{
			clr = tm->color(HIGHLIGHT);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.4);
			cr->fill_preserve();
		}
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke_preserve();
		cr->clip();

		int stepSize = controller_->getWaveformWidth() / get_width();
		if (stepSize < 1)
		{
			stepSize = 1;
		}
		std::vector<float>* samples = controller_->getSamples(waveform_, stepSize);
		float ppS = (float) get_width() / samples->size();
		for (int j = 1; j < samples->size(); j++)
		{
			cr->move_to(ppS * j, get_height() * 0.5 - (get_height() - tm->lineThick()) * 0.5 * (*samples)[j]);
			cr->line_to(ppS * (j - 1), get_height() * 0.5 - (get_height() - tm->lineThick()) * 0.5 * (*samples)[j - 1]);
		}
		cr->move_to(ppS * (controller_->getWaveformWidth() - 1), get_height() * 0.5 - (get_height() - tm->lineThick()) * 0.5 * (*samples)[samples->size() - 1]);
		cr->line_to(get_width(), get_height() * 0.5 - (get_height() - tm->lineThick()) * 0.5 * (*samples)[0]);
		//cairo_set_line_join(cairoCache, CAIRO_LINE_JOIN_ROUND);
		cr->set_line_width(tm->lineThick());
		clr = tm->color(FG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->stroke();
		cr->reset_clip();
		if (samples)
		{
			delete samples;
		}
		samples = nullptr;
	}
	if (dropLocation_ >= 0)
	{
		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->begin_new_path();
		int height = (get_height() - tm->lineThick()) * dropLocation_ + 0.5 * tm->lineThick();
		cr->move_to(0, height);
		cr->line_to(get_width(), height);
		cr->set_line_width(tm->lineThick());
		cr->stroke();
	}
}

bool CR42YWavetableViewItem::custom_button_press_event(GdkEventButton* event)
{
	if (event->button == 1)
	{
		mouseDown_ = true;
	}
	return false;
}

bool CR42YWavetableViewItem::custom_button_release_event(GdkEventButton* event)
{
	if (event->button == 1 && mouseDown_ && !inDnD_)
	{
		controller_->selectWaveform(waveform_);
		mouseDown_ = false;
	}
	return false;
}

void CR42YWavetableViewItem::removeBtnClicked()
{
	if (controller_)
	{
		controller_->removeWaveform(waveform_);
	}
}

void CR42YWavetableViewItem::on_drag_begin(
		const Glib::RefPtr<Gdk::DragContext>& context)
{
	Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, get_width(), get_height());
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

	draw(cr);

	unsigned char* data = surface->get_data();
	int pixels = surface->get_width() * surface->get_height();
	for (int i = 0; i < pixels * 4; i += 4)
	{
		data[i] = data[i + 2] ^ data[i];
		data[i + 2] = data[i + 2] ^ data[i];
		data[i] = data[i + 2] ^ data[i];
	}

	Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_data(data, Gdk::COLORSPACE_RGB, true, 8, surface->get_width(), surface->get_height(), surface->get_stride());
	//copy because otherwise contents are lost
	context->set_icon(pixbuf->copy(), 0, 0);

	inDnD_ = true;
}

void CR42YWavetableViewItem::on_drag_data_get(
		const Glib::RefPtr<Gdk::DragContext>& context,
		Gtk::SelectionData& selection_data, guint info, guint time)
{
	//gtk_selection_data_set(selection_data.gobj(), gdk_atom_intern_static_string("CR42YWavetableViewItem"), 32, (const guchar*) this, sizeof(gpointer));
	const std::string type = "CR42YWavetableViewItem";
	CR42YWavetableViewItem* self = this;
	const unsigned char* data = (unsigned char*) &self;

	selection_data.set(type, 32, data, sizeof(const unsigned char*));
}

void CR42YWavetableViewItem::on_drag_data_received(
		const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
		const Gtk::SelectionData& selection_data, guint info, guint time)
{
	CR42YWavetableViewItem* source = *((CR42YWavetableViewItem**) selection_data.get_data());
	if (source == this)
	{
		return;
	}
	bool isSourceSelected = controller_->selectedWaveform() == source->waveform_;

	int newPos = waveform_;
	if (y > get_height() / 2)
	{
		newPos++;
	}
	controller_->moveWaveform(source->waveform_, newPos);

	if (isSourceSelected)
	{
		int selectPos = newPos;
		if (newPos > source->waveform_)
		{
			selectPos--;
		}
		controller_->selectWaveform(selectPos);
	}
	else if (newPos <= controller_->selectedWaveform() && source->waveform_ > controller_->selectedWaveform())
	{
		controller_->selectWaveform(controller_->selectedWaveform() + 1);
	}
	else if (newPos > controller_->selectedWaveform() && source->waveform_ <= controller_->selectedWaveform())
	{
		controller_->selectWaveform(controller_->selectedWaveform() - 1);
	}
}

bool CR42YWavetableViewItem::on_drag_motion(
		const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time)
{
	if (x > 0 && x < get_width() && y > 0 && y < get_height())
	{
		dropLocation_ = y > get_height() / 2;
		queue_draw();
	}
}

void CR42YWavetableViewItem::on_drag_leave(
		const Glib::RefPtr<Gdk::DragContext>& context, guint time)
{
	dropLocation_ = -1;
	queue_draw();
}

void CR42YWavetableViewItem::on_drag_end(const Glib::RefPtr<Gdk::DragContext>& context)
{
	inDnD_ = false;
}

} /* namespace cr42y */
