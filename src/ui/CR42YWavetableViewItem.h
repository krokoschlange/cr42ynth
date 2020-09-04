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
