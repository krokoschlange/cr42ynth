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

#ifndef CR42Y_CR42YAUTOMATIONSAMPLEEDITOR_H
#define CR42Y_CR42YAUTOMATIONSAMPLEEDITOR_H

#include <gtkmm.h>
#include <CR42YWidget.h>

namespace cr42y
{
	class AutomationEditController;

class CR42YAutomationSampleEditor : public Gtk::Widget, public CR42YWidget
{
public:
	CR42YAutomationSampleEditor(CR42YUI* ui, AutomationEditController* controller);

	virtual ~CR42YAutomationSampleEditor();
	
	void setGridX(int gridX);
	void setGridY(int gridY);

protected:
	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_realize();
	
	bool on_button_press(GdkEventButton* event);
	bool on_button_release(GdkEventButton* event);
	bool on_motion_notify(GdkEventMotion* event);
	
private:
	Glib::RefPtr<Gdk::Window> window_;
	
	AutomationEditController* controller_;
	
	bool grabbed_;
	size_t grabbedHandle_;
	bool isSectionHandle_;
	int grabX_;
	int grabY_;
	bool reset_;
	
	float blobSize_;
	float blobGrabSize_;
	
	int gridX_;
	int gridY_;
	
	void dataUpdateCallback();
	void selectionUpdateCallback(size_t selected);
};

}

#endif // CR42Y_CR42YAUTOMATIONSAMPLEEDITOR_H
