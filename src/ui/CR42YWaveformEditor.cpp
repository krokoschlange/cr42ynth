/*
 * CR42YWaveformEditor.cpp
 *
 *  Created on: 04.08.2020
 *      Author: fabian
 */

#include "CR42YWaveformEditor.h"
#include "CR42YTheme.h"
#include "WavetableEditController.h"
#include "CR42YUI.h"
#include "CR42YButton.h"
#include "helpers.h"

#include <iostream>

namespace cr42y
{

CR42YWaveformEditor::CR42YWaveformEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWaveformEditor"),
		CR42YRelativeContainer(ui),
		controller_(nullptr)
{
	setDrawBorder(true);
	setDrawBG(false);

	Gtk::Widget::add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON1_MOTION_MASK);
	signal_button_press_event().connect(sigc::mem_fun(this, &CR42YWaveformEditor::on_button_press));
	signal_button_release_event().connect(sigc::mem_fun(this, &CR42YWaveformEditor::on_button_release));
	signal_motion_notify_event().connect(sigc::mem_fun(this, &CR42YWaveformEditor::on_motion_notify));
}
CR42YWaveformEditor::~CR42YWaveformEditor()
{
}

void CR42YWaveformEditor::setController(WavetableEditController* controller)
{
	controller_ = controller;
}

sigc::signal<void> CR42YWaveformEditor::waveformUpdateSignal()
{
	return waveformUpdateSignal_;
}

void CR42YWaveformEditor::on_realize()
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

bool CR42YWaveformEditor::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win && controller_)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->rectangle(0, 0, get_width(), get_height());
		cr->fill();

		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(), tm->cornerRadius());
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke_preserve();
		cr->clip();

		int gridX = controller_->gridX();
		int gridY = controller_->gridY();

		for (int x = 0; x < gridX; x++)
		{
			cr->move_to(((float) get_width() / gridX) * x, 0);
			cr->line_to(((float) get_width() / gridX) * x, get_height());
		}
		if (gridX > 0)
		{
			cr->move_to(get_width(), 0);
			cr->line_to(get_width(), get_height());

			clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());
			cr->stroke();
		}

		for (int y = 0; y < gridY; y++)
		{
			cr->move_to(0, ((float) get_height() / gridY) * y);
			cr->line_to(get_width(), ((float) get_height() / gridY) * y);
		}
		if (gridY > 0)
		{
			cr->move_to(0, get_height());
			cr->line_to(get_width(), get_height());

			clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());
			cr->stroke();
		}

		std::vector<float>* samples = nullptr;
		int stepSize = controller_->getWaveformWidth() / get_width();
		stepSize = stepSize > 1 ? stepSize : 1;
		samples = controller_->getSamples(controller_->selectedWaveform(), stepSize);

		float pixelPerSample = (float) get_width() / samples->size();

		stepSize = samples->size() / get_width();
		if (stepSize < 1)
		{
			stepSize = 1;
		}

		for (int i = 1; i < samples->size(); i++)
		{
			cr->move_to(pixelPerSample * i, get_height() * -0.5 * (*samples)[i] + get_height() / 2);
			cr->line_to(pixelPerSample * (i - stepSize), get_height() * -0.5 * (*samples)[i - stepSize] + get_height() / 2);
		}
		cr->move_to(pixelPerSample * (samples->size() - 1), get_height() * -0.5 * (*samples)[samples->size() - 1] + get_height() / 2);
		cr->line_to(get_width(), get_height() * -0.5 * (*samples)[0] + get_height() / 2);
		cr->set_line_join(Cairo::LINE_JOIN_ROUND);
		cr->set_line_width(tm->lineThick());
		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);

		cr->stroke();

		int sel = controller_->getSelectedPart();
		std::vector<std::pair<float, float>> parts;
		parts = controller_->getVisibleAreas(sel);

		for (int i = 0; i < parts.size(); i++)
		{
			cr->rectangle(parts[i].first * get_width(), 0, (parts[i].second - parts[i].first) * get_width(), get_height());

			clr = tm->color(FG_DARK);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3] * 0.4);

			cr->fill();

			if (parts[i].second * get_width() - parts[i].first * get_width() < 5)
			{
				cr->move_to(parts[i].first * get_width() - 10, 5);
				cr->line_to(parts[i].first * get_width(), get_height() / 2);
				cr->line_to(parts[i].first * get_width() - 10, get_height() - 5);
				cr->close_path();
				clr = tm->color(FG);
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
				cr->fill();

				cr->move_to(parts[i].second * get_width() + 10, 5);
				cr->line_to(parts[i].second * get_width(), get_height() / 2);
				cr->line_to(parts[i].second * get_width() + 10, get_height() - 5);
				cr->close_path();
				cr->fill();
			}
		}

		/*cairo_set_font_size(cairoCache, 15);
		 cairo_select_font_face(cairoCache, "impact", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		 cairo_text_extents_t extents;
		 theme_->color(cairoCache, Avtk::FG);

		 cairo_text_extents(cairoCache, "grid X", &extents);
		 cairo_move_to(cairoCache, w() - 80 - extents.width, h() - 5);
		 cairo_show_text(cairoCache, "gridX");

		 cairo_text_extents(cairoCache, std::to_string(gridX).c_str(), &extents);
		 cairo_move_to(cairoCache, w() - 40 - extents.width / 2, h() - 5);
		 cairo_show_text(cairoCache, std::to_string(gridX).c_str());

		 cairo_text_extents(cairoCache, "grid Y", &extents);
		 cairo_move_to(cairoCache, w() - 80 - extents.width, h() - 25);
		 cairo_show_text(cairoCache, "gridY");

		 cairo_text_extents(cairoCache, std::to_string(gridY).c_str(), &extents);
		 cairo_move_to(cairoCache, w() - 40 - extents.width / 2, h() - 25);
		 cairo_show_text(cairoCache, std::to_string(gridY).c_str());*/

		if (samples)
		{
			delete samples;
		}
	}

	return CR42YRelativeContainer::on_expose_event(event);
}

bool CR42YWaveformEditor::on_button_press(GdkEventButton* event)
{
	if (controller_)
	{
		if (event->button == 1)
		{
			controller_->useToolAction(event->x, event->y, get_width(), get_height());
			updateButtons();
			gtk_grab_add(Gtk::Widget::gobj());
			queue_draw();
			return true;
		}
		else if (event->button == 3)
		{
			controller_->selectPartAction(event->x, event->y, get_width(), get_height());
			updateButtons();
			queue_draw();
			return true;
		}
	}
	return false;
}

bool CR42YWaveformEditor::on_button_release(GdkEventButton* event)
{
	if (event->button == 1 && controller_)
	{
		controller_->dropToolAction();
		updateButtons();
		gtk_grab_remove(Gtk::Widget::gobj());
		waveformUpdateSignal().emit();
		return true;
	}
	return false;
}

bool CR42YWaveformEditor::on_motion_notify(GdkEventMotion* event)
{
	if (controller_)
	{
		controller_->toolMoveAction(event->x, event->y, get_width(), get_height());
		updateButtons();
		queue_draw();
		return true;
	}
	return false;
}

void CR42YWaveformEditor::updateButtons()
{
	std::vector<Gtk::Widget*> children = get_children();

	for (int i = 0; i < children.size(); i++)
	{
		Gtk::Widget* child = children[i];
		remove(*child);
		delete child;
	}
	//std::vector<WaveformPart*>* wf = editor->getEditData()->getWaveform(editor->getWTPos());
	//int sel = editor->getSelected(editor->getWTPos());
	int sel = controller_->getSelectedPart();
	//if (0 < sel && sel < wf->size())
	//{
	//Avtk::Button* rmvBtn = new Avtk::Button(ui, (*wf)[sel]->getStart() * editor->getEditData()->getWidth() + 5, 5, 10, 10, "X");

	if (sel > 0)
	{
		std::string path = std::string(ui_->resourceRoot());

		int padding = 5;
		int buttonSize = 25;

		std::vector<std::pair<float, float>> visAreas = controller_->getVisibleAreas(sel);
		if (visAreas.size() > 0)
		{
			Cairo::RefPtr<Cairo::Surface> min = Cairo::ImageSurface::create_from_png("build/../media/minus.png");
			double btnX = visAreas[0].first;
			if (btnX + padding + buttonSize > get_width())
			{
				btnX = get_width() - padding - buttonSize;
			}
			//CRSurfaceButton* rmvBtn = new CRSurfaceButton(ui, x() + btnX, y() + 5, 15, 15, "-", min, 20, 20, min, 20, 20);
			CR42YButton* rmvBtn = new CR42YButton(ui_);
			//rmvBtn->setText("-");
			rmvBtn->setSurfActive(min);
			rmvBtn->setSurfInactive(min);
			rmvBtn->signalClicked().connect(sigc::bind<int>(sigc::mem_fun(this, &CR42YWaveformEditor::removePartCallback), sel));

			//removeBtns.push_back(std::pair<int, CRSurfaceButton*>(sel, rmvBtn));
			put(rmvBtn, btnX, 0, buttonSize, buttonSize, padding, padding);
			rmvBtn->show();
			//rmvBtn->theme(ui->theme(1));
		}

		/*cairo_surface_t* left = cairo_image_surface_create_from_png((path + "../media/left.png").c_str());
		resizeLeft = new CRSurfaceButton(ui, x() + controller->getPartStart(sel) * w() - 15, y() + h() / 2 - 15, 15, 30, "<", left, 20, 50, left, 20, 50);
		add (resizeLeft);
		cairo_surface_destroy(left);

		cairo_surface_t* right = cairo_image_surface_create_from_png((path + "../media/right.png").c_str());
		resizeRight = new CRSurfaceButton(ui, x() + controller->getPartEnd(sel) * w(), y() + h() / 2 - 15, 15, 30, ">", right, 20, 50, right, 20, 50);
		add (resizeRight);
		cairo_surface_destroy(right);*/
	}
}

void CR42YWaveformEditor::removePartCallback(int part)
{
	if (controller_)
	{
		controller_->removePart(part);
		updateButtons();
		waveformUpdateSignal().emit();
	}
}

} /* namespace cr42y */
