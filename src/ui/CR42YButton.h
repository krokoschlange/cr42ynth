/*
 * CR42YButton.h
 *
 *  Created on: 25.06.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YBUTTON_H_
#define SRC_UI_CR42YBUTTON_H_

#include "CR42YLabel.h"

namespace cr42y
{

class CR42YButton : public CR42YLabel
{
public:
	CR42YButton(CR42YUI* ui);
	virtual ~CR42YButton();

	sigc::signal<void> signalClicked();

protected:
	bool on_button_press(GdkEventButton* event);
	bool on_button_release(GdkEventButton* event);

private:
	sigc::signal<void> signalClicked_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YBUTTON_H_ */
