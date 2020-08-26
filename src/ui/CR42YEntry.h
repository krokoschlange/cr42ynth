/*
 * CR42YEntry.h
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YENTRY_H_
#define SRC_UI_CR42YENTRY_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YEntry : public Gtk::Entry, public CR42YWidget
{
public:
	CR42YEntry(CR42YUI* ui);
	virtual ~CR42YEntry();
	
	void setDrawCursor(bool drawCursor);
	void setDrawSelection(bool drawSelection);

protected:
	bool on_expose_event(GdkEventExpose* event);
	void on_size_allocate(Gtk::Allocation& alloc);
	
private:
	void drawText();
	void drawCursor();
	
	bool drawCursor_;
	bool drawSelection_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YENTRY_H_ */
