/*
 * CR42YWavetableView.h
 *
 *  Created on: 07.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWAVETABLEVIEW_H_
#define SRC_UI_CR42YWAVETABLEVIEW_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class WavetableEditController;

class CR42YWavetableView : public CR42YRelativeContainer
{
public:
	CR42YWavetableView(CR42YUI* ui);
	virtual ~CR42YWavetableView();

	void setController(WavetableEditController* controller);
	void update();

	sigc::signal<void> waveformRemovedSignal();

protected:
	void on_realize();
	void on_size_request(Gtk::Requisition* requisition);

	void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

private:
	WavetableEditController* controller_;

	Glib::RefPtr<Gdk::Window> window_;

	sigc::signal<void> waveformRemovedSignal_;

	int boxSize_;

	void removeWaveformCallback(int waveform);

	void selectWaveformCallback(int waveform);
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEVIEW_H_ */
