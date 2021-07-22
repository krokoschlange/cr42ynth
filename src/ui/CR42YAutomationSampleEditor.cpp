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

#include "CR42YAutomationSampleEditor.h"

#include "AutomationEditController.h"
#include "CR42YTheme.h"
#include "helpers.h"
#include "Control.h"

namespace cr42y
{

CR42YAutomationSampleEditor::CR42YAutomationSampleEditor(CR42YUI* ui, AutomationEditController* controller) :
		Glib::ObjectBase("CR42YAutomationSampleEditor"),
		Gtk::Widget(),
		CR42YWidget(ui),
		controller_(controller),
		grabbed_(false),
		reset_(false),
		blobSize_(0.03),
		blobGrabSize_(0.05),
		gridX_(0),
		gridY_(0)
{
	set_flags(Gtk::NO_WINDOW);
	
	Gtk::Widget::add_events(
		Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
		| Gdk::BUTTON1_MOTION_MASK);
	signal_button_press_event().connect(
		sigc::mem_fun(this, &CR42YAutomationSampleEditor::on_button_press));
	signal_button_release_event().connect(
		sigc::mem_fun(this, &CR42YAutomationSampleEditor::on_button_release));
	signal_motion_notify_event().connect(
		sigc::mem_fun(this, &CR42YAutomationSampleEditor::on_motion_notify));
	
	controller->dataChangedSignal().connect(sigc::mem_fun(this, &CR42YAutomationSampleEditor::dataUpdateCallback));
	controller->selectionChangedSignal().connect(sigc::mem_fun(this, &CR42YAutomationSampleEditor::selectionUpdateCallback));
}

CR42YAutomationSampleEditor::~CR42YAutomationSampleEditor()
{

}

void CR42YAutomationSampleEditor::setGridX(int gridX)
{
	gridX_ = gridX;
	queue_draw();
}

void CR42YAutomationSampleEditor::setGridY(int gridY)
{
	gridY_ = gridY;
	queue_draw();
}

void CR42YAutomationSampleEditor::on_realize()
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
		
		window_ = Gdk::Window::create(get_window(), &attributes,
									  GDK_WA_X | GDK_WA_Y);
		
		unset_flags(Gtk::NO_WINDOW);
		set_window(window_);
		
		window_->set_user_data(gobj());
	}
}

bool CR42YAutomationSampleEditor::on_expose_event(GdkEventExpose*)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win && controller_)
	{
		std::vector<float> samples;
		controller_->samples(samples, get_width());
		
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		
		cr->rectangle(0, 0, get_width(), get_height());
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->fill();
		
		cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(),
								tm->cornerRadius(), tm->lineThick());
		clr = tm->color(FG);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->stroke();
		
		for (int x = 0; x < gridX_; x++)
		{
			cr->move_to(((float) get_width() / gridX_) * x, 0);
			cr->line_to(((float) get_width() / gridX_) * x, get_height());
		}
		if (gridX_ > 0)
		{
			cr->move_to(get_width(), 0);
			cr->line_to(get_width(), get_height());
			
			clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());
			cr->stroke();
		}
		
		for (int y = 0; y < gridY_; y++)
		{
			cr->move_to(0, ((float) get_height() / gridY_) * y);
			cr->line_to(get_width(), ((float) get_height() / gridY_) * y);
		}
		if (gridY_ > 0)
		{
			cr->move_to(0, get_height());
			cr->line_to(get_width(), get_height());
			
			clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());
			cr->stroke();
		}
		
		for (size_t i = 1; i < samples.size(); i++)
		{
			cr->move_to(i,
						get_height() * -1 * samples[i] + get_height());
			cr->line_to(i - 1,
						get_height() * -1 * samples[i - 1] + get_height());
		}
		clr = tm->color(HIGHLIGHT);
		cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
		cr->set_line_width(tm->lineThick());
		cr->stroke();
		
		for (size_t i = 0; i < controller_->sectionAmount(); i++)
		{
			float x = 0;
			float y = 0;
			controller_->sectionHandle(&x, &y, i);
			cr->arc(get_width() * x, get_height() * (1 - y),
					get_height() * blobSize_, 0, M_PI * 2
			);
			cr->set_line_width(tm->lineThick() * 2);
			cr->stroke();
		}
		for (size_t i = 0; i < controller_->sectionAmount() - 1; i++)
		{
			float x1 = 0;
			float x2 = 0;
			float y = controller_->handlePosition(i);
			controller_->sectionHandle(&x1, nullptr, i);
			controller_->sectionHandle(&x2, nullptr, i + 1);
			float x = (x1 + x2) / 2;
			
			cr->arc(get_width() * x, get_height() * (1 - y),
					get_height() * blobSize_, 0, M_PI * 2
			);
			cr->fill();
		}
		if (controller_->getType(controller_->selectedAutomation()) == 0)
		{
			size_t sustain = controller_->sustainControl()->getValue();
			float x = 0;
			controller_->sectionHandle(&x, nullptr, sustain);
			cr->move_to(get_width() * x , 0);
			cr->line_to(get_width() * x , get_height());
			clr = tm->color(HIGHLIGHT);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());
			cr->stroke();
		}
	}
	return false;
}

bool CR42YAutomationSampleEditor::on_button_press(GdkEventButton* event)
{
	if (!controller_)
	{
		return false;
	}
	float x = event->x;
	float y = get_height() - event->y;
	if (event->button == 1 && event->type == GDK_BUTTON_PRESS)
	{
		grabbed_ = false;
		float closest = -1;
		for (size_t i = 0; i < controller_->sectionAmount(); i++)
		{
			
			float hx = 0;
			float hy = 0;
			controller_->sectionHandle(&hx, &hy, i);
			hx *= get_width();
			hy *= get_height();
			float dist = std::sqrt((x - hx) * (x - hx) + (y - hy) * (y - hy));
			if (dist < get_height() * blobGrabSize_ && (closest < 0 || dist < closest))
			{
				closest = dist;
				grabbed_ = true;
				grabbedHandle_ = i;
				isSectionHandle_ = true;
			}
		}
		for (size_t i = 0; i < controller_->sectionAmount() - 1; i++)
		{
			float hx1 = 0;
			float hx2 = 0;
			controller_->sectionHandle(&hx1, nullptr, i);
			controller_->sectionHandle(&hx2, nullptr, i + 1);
			float hx = (hx1 + hx2) / 2 * get_width();
			float hy = controller_->handlePosition(i) * get_height();
			float dist = std::sqrt((x - hx) * (x - hx) + (y - hy) * (y - hy));
			if (dist < get_height() * blobGrabSize_ && (closest < 0 || dist < closest))
			{
				closest = dist;
				grabbed_ = true;
				grabbedHandle_ = i;
				isSectionHandle_ = false;
			}
		}
		grabX_ = x;
		grabY_ = y;
		if (!grabbed_)
		{
			float relX = x / get_width();
			float relY = y / get_height();
			if (gridX_ > 0)
			{
				relX = (float) ((int) (relX * gridX_ + 0.5)) / gridX_;
			}
			if (gridY_ > 0)
			{
				relY = (float) ((int) (relY * gridY_ + 0.5)) / gridY_;
			}
			grabbed_ = true;
			isSectionHandle_ = true;
			grabbedHandle_ = controller_->createSection(relX, relY);
		}
	}
	else if (event->button == 3 or (event->button == 1 and event->type == GDK_2BUTTON_PRESS))
	{
		grabbed_ = false;
		float closest = -1;
		size_t section = 0;
		for (size_t i = 0; i < controller_->sectionAmount(); i++)
		{
			float hx = 0;
			float hy = 0;
			controller_->sectionHandle(&hx, &hy, i);
			hx *= get_width();
			hy *= get_height();
			float dist = std::sqrt((x - hx) * (x - hx) + (y - hy) * (y - hy));
			if (dist < get_height() * blobGrabSize_ && (closest < 0 || dist < closest))
			{
				closest = dist;
				section = i;
			}
		}
		if (closest > 0)
		{
			controller_->removeSection(section);
		}
	}
	return true;
}

bool CR42YAutomationSampleEditor::on_motion_notify(GdkEventMotion* event)
{
	if (grabbed_ && controller_)
	{
		int diffY = (get_height() - event->y) - grabY_;
		grabX_ = event->x;
		grabY_ = get_height() - event->y;
		
		//ModulationData::Section& section = data_->getSections()[grabbedHandle_];
		
		float oldX = 0;
		float oldY = 0;
		if (isSectionHandle_)
		{
			controller_->sectionHandle(&oldX, &oldY, grabbedHandle_);
		}
		else
		{
			float x1 = 0;
			float x2 = 0;
			controller_->sectionHandle(&x1, nullptr, grabbedHandle_);
			controller_->sectionHandle(&x2, nullptr, grabbedHandle_);
			oldX = (x1 + x2) / 2;
			oldY = controller_->handle(grabbedHandle_);
		}
		
		
		if (isSectionHandle_)
		{
			float newX = (float) event->x / get_width();
			float newY = 1 - (float) event->y / get_height();
			if (gridX_ > 0)
			{
				newX = (float) ((int) (newX * gridX_ + 0.5)) / gridX_;
			}
			if (gridY_ > 0)
			{
				newY = (float) ((int) (newY * gridY_ + 0.5)) / gridY_;
			}
			controller_->moveSection(grabbedHandle_, newX);
			controller_->moveSectionHandle(grabbedHandle_, newY);
		}
		else
		{
			float relDiffY = (float) diffY / get_height();
			//float sectionHeight = controller_->getSectionHeight(grabbedHandle_);
			float newY = oldY - relDiffY;// / sectionHeight;
			controller_->moveHandle(grabbedHandle_, newY);
			
			if (reset_)
			{
				Glib::RefPtr<Gdk::Window> win = get_window();
				int x = 0;
				int y = 0;
				
				win->get_root_coords(win->get_width() / 2, win->get_height() / 2, x, y);
				grabX_ -= event->x - win->get_width() / 2;
				grabY_ += event->y - win->get_height() / 2;
				win->get_display()->warp_pointer(win->get_screen(), x, y);
				reset_ = false;
			}
			else
			{
				reset_ = true;
			}
		}
		return true;
	}
	return false;
}

bool CR42YAutomationSampleEditor::on_button_release(GdkEventButton* event)
{
	grabbed_ = false;
	gdk_pointer_ungrab(event->time);
	return false;
}

void CR42YAutomationSampleEditor::dataUpdateCallback()
{
	queue_draw();
}

void CR42YAutomationSampleEditor::selectionUpdateCallback(size_t)
{
	queue_draw();
}

}


