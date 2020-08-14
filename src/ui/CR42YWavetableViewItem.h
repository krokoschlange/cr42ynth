/*
 * CR42YWavetableViewItem.h
 *
 *  Created on: 09.08.2020
 *      Author: fabian
 */

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
	CR42YWavetableViewItem(CR42YUI* ui, bool button);
	virtual ~CR42YWavetableViewItem();

	sigc::signal<void> removeButtonClickedSignal();
	sigc::signal<void> waveformSelectedSignal();

	void setController(WavetableEditController* controller);
	void setWaveform(int waveform);
	int waveform();

protected:
	void on_realize();
	bool on_expose_event(GdkEventExpose* event);
	bool on_button_release_event(GdkEventButton* event);

	void draw(Cairo::RefPtr<Cairo::Context> cr);

	void on_drag_begin(const Glib::RefPtr<Gdk::DragContext>& context);
	void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
	void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
	bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time);
	void on_drag_leave(const Glib::RefPtr<Gdk::DragContext>& context, guint time);

private:
	Glib::RefPtr<Gdk::Window> window_;

	sigc::signal<void> waveformSelectedSignal_;

	CR42YButton* removeBtn_;

	WavetableEditController* controller_;
	int waveform_;

	int dropLocation_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEVIEWITEM_H_ */
