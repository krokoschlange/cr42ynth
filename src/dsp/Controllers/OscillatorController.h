/*
 * OscillatorController.h
 *
 *  Created on: 02.06.2019
 *      Author: fabian
 */

#ifndef SRC_DSP_CONTROLLERS_OSCILLATORCONTROLLER_H_
#define SRC_DSP_CONTROLLERS_OSCILLATORCONTROLLER_H_

#include "Controller.h"

namespace cr42y
{

class OscillatorController : public Controller
{
public:
	OscillatorController();
	virtual ~OscillatorController();

	void nextFrame();
};

} /* namespace cr42y */

#endif /* SRC_DSP_CONTROLLERS_OSCILLATORCONTROLLER_H_ */
