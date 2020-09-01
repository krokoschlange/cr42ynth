/*
 * CR42YIntegerEntry.h
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YINTEGERENTRY_H_
#define SRC_UI_CR42YINTEGERENTRY_H_

#include "CR42YLabel.h"

namespace cr42y
{

class CR42YIntegerEntry : public CR42YLabel
{
public:
	CR42YIntegerEntry(CR42YUI* ui);
	virtual ~CR42YIntegerEntry();

	void setValue(int value);
	int value();
	void setMin(int min, bool use = true);
	int min();
	void setMax(int max, bool use = true);
	int max();

	sigc::signal<void, int> signalChanged();

protected:
	bool on_button_press_event(GdkEventButton* event);
	bool on_key_press_event(GdkEventKey* event);

private:
	Glib::RefPtr<Gdk::Window> window_;

	sigc::signal<void, int> signalChanged_;

	int value_;
	int min_;
	bool useMin_;
	int max_;
	bool useMax_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YINTEGERENTRY_H_ */
