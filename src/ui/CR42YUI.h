/*
 * CR42YUI.h
 *
 *  Created on: 04.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YUI_H_
#define SRC_UI_CR42YUI_H_

#include "CR42YWidget.h"
#include <string>

namespace cr42y
{

class CR42YUI : public CR42YWidget
{
public:
	CR42YUI();
	virtual ~CR42YUI();

	void setResourceRoot(std::string resourceRoot);
	std::string resourceRoot();

private:
	std::string resourceRoot_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YUI_H_ */
