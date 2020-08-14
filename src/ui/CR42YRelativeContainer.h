/*
 * CR42YRelativeContainer.h
 *
 *  Created on: 30.07.2020
 *      Author: fabian
 */

#ifndef SRC_UI_WTEDITOR_CR42YRELATIVECONTAINER_H_
#define SRC_UI_WTEDITOR_CR42YRELATIVECONTAINER_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

struct CR42YRelativeChild
{
	Gtk::Widget* widget;
	double x;
	double y;
	double w;
	double h;
	int padLeft;
	int padTop;
	int padRight;
	int padBottom;
};

class CR42YRelativeContainer : public Gtk::Container, public CR42YWidget
{
public:
	CR42YRelativeContainer(CR42YUI* ui);
	virtual ~CR42YRelativeContainer();

	void put(Gtk::Widget* child, double x, double y, double w, double h, int padLeft = 0, int padTop = 0, int padRight = 0, int padBottom = 0);
	void move(Gtk::Widget* child, double x, double y, double w, double h, int padLeft = 0, int padTop = 0, int padRight = 0, int padBottom = 0);
	void on_remove(Gtk::Widget* child);
	void on_add(Gtk::Widget* child);


protected:
	void on_realize();
	bool on_expose_event(GdkEventExpose* event);
	void on_size_allocate(Gtk::Allocation& alloc);
	void on_size_request(Gtk::Requisition* requisition);
	void forall_vfunc(gboolean include_internals, GtkCallback callback, gpointer callback_data);

	CR42YRelativeChild* getChild(Gtk::Widget* child);

private:

	std::vector<CR42YRelativeChild> children_;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_CR42YRELATIVECONTAINER_H_ */
