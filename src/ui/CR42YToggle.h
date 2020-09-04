/*
 * CR42YToggle.h
 *
 *  Created on: 03.09.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YTOGGLE_H_
#define SRC_UI_CR42YTOGGLE_H_

#include "CR42YLabel.h"

namespace cr42y
{

class CR42YToggle : public CR42YLabel
{
public:
	CR42YToggle(CR42YUI* ui);
	virtual ~CR42YToggle();

	sigc::signal<void> signalClicked();

protected:
	bool on_button_press(GdkEventButton* event);
	bool on_button_release(GdkEventButton* event);

private:
	sigc::signal<void> signalClicked_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YTOGGLE_H_ */
