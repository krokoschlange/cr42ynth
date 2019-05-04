/*
 * CR42YnthUI.h
 *
 *  Created on: 03.05.2019
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YNTHUI_H_
#define SRC_UI_CR42YNTHUI_H_

#include "avtk.hxx"

namespace cr42y
{

class CR42YnthUI : public Avtk::UI
{
public:
	CR42YnthUI(PuglNativeWindow parent);
	virtual ~CR42YnthUI();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YNTHUI_H_ */
