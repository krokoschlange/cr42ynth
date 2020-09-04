/*
 * CR42YWidget.cpp
 *
 *  Created on: 28.06.2020
 *      Author: fabian
 */

#include "CR42YWidget.h"
#include "CR42YnthUI.h"

namespace cr42y
{

CR42YWidget::CR42YWidget(CR42YUI* ui) :
		ui_(ui),
		theme_(nullptr)
{
	if (ui)
	{
		setTheme(ui->theme());
	}
}

CR42YWidget::~CR42YWidget()
{
}

CR42YTheme* CR42YWidget::theme()
{
	return theme_;
}

void CR42YWidget::setTheme(CR42YTheme* theme)
{
	theme_ = theme;
}

CR42YUI* CR42YWidget::ui()
{
	return ui_;
}

} /* namespace cr42y */
