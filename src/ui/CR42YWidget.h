/*
 * CR42YWidget.h
 *
 *  Created on: 28.06.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWIDGET_H_
#define SRC_UI_CR42YWIDGET_H_

namespace cr42y
{

class CR42YTheme;
class CR42YUI;

class CR42YWidget
{
public:
	CR42YWidget(CR42YUI* ui);
	virtual ~CR42YWidget();

	CR42YTheme* theme();
	void setTheme(CR42YTheme* theme);

protected:
	CR42YUI* ui_;
	CR42YTheme* theme_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWIDGET_H_ */
