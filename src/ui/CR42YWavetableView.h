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
class CR42YWavetableViewItem;

class CR42YWavetableView : public CR42YRelativeContainer
{
public:
	CR42YWavetableView(CR42YUI* ui);
	virtual ~CR42YWavetableView();

	void setController(WavetableEditController* controller);
	void update();

protected:
	void on_realize();
	void on_size_request(Gtk::Requisition* requisition);

	void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

private:
	WavetableEditController* controller_;

	std::vector<CR42YWavetableViewItem*> items_;

	Glib::RefPtr<Gdk::Window> window_;

	int boxSize_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEVIEW_H_ */
