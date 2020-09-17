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


#ifndef SRC_UI_CR42YWAVETABLEVIEWITEM_H_
#define SRC_UI_CR42YWAVETABLEVIEWITEM_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{
class CR42YButton;
class WavetableEditController;

class CR42YWavetableViewItem : public CR42YRelativeContainer
{
public:
	CR42YWavetableViewItem(CR42YUI* ui);
	virtual ~CR42YWavetableViewItem();

	void setController(WavetableEditController* controller);
	void setWaveform(int waveform);
	int waveform();

	void setShowButton(bool show);

protected:
	void on_realize();
	bool on_expose_event(GdkEventExpose* event);
	bool custom_button_press_event(GdkEventButton* event);
	bool custom_button_release_event(GdkEventButton* event);
	void removeBtnClicked();

	void draw(Cairo::RefPtr<Cairo::Context> cr);

	void on_drag_begin(const Glib::RefPtr<Gdk::DragContext>& context);
	void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
	void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
	bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time);
	void on_drag_leave(const Glib::RefPtr<Gdk::DragContext>& context, guint time);
	void on_drag_end(const Glib::RefPtr<Gdk::DragContext>& context);

private:
	Glib::RefPtr<Gdk::Window> window_;

	CR42YButton* removeBtn_;

	WavetableEditController* controller_;
	int waveform_;

	int dropLocation_;

	bool mouseDown_;
	bool inDnD_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEVIEWITEM_H_ */
