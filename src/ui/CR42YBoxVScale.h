/*
 * CR42YBoxVScale.h
 *
 *  Created on: 28.07.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YBOXVSCALE_H_
#define SRC_UI_CR42YBOXVSCALE_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YBoxVScale : public Gtk::Widget, public CR42YWidget
{
public:
	CR42YBoxVScale(CR42YUI* ui);
	virtual ~CR42YBoxVScale();

	bool doubleSided();
	void setDoubleSided(bool ds);

	double value();
	void setValue(double value, bool trigger = true);

	sigc::signal<void, double> signalChanged();

protected:

	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_realize();

	bool on_button_press(GdkEventButton* event);
	bool on_button_release(GdkEventButton* event);
	bool on_motion_notify(GdkEventMotion* event);

private:
	Glib::RefPtr<Gdk::Window> window_;

	sigc::signal<void, double> signalChanged_;

	bool doubleSided_;
	double value_;
	double preClickValue_;
	double prePreClickValue_;
	double oldValue_;
	double defaultValue_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YBOXVSCALE_H_ */
