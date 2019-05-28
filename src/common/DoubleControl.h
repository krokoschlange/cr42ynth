/*
 * DoubleControl.h
 *
 *  Created on: 28.05.2019
 *      Author: fabian
 */

#ifndef SRC_COMMON_DOUBLECONTROL_H_
#define SRC_COMMON_DOUBLECONTROL_H_

#include <lv2/atom/atom.h>

#include "DefinitionHandler.h"
#include "PortCommunication/MessageReceiver.h"

namespace cr42y
{
class MessageReceiver;

class DoubleControl: public MessageReceiver
{
public:
	DoubleControl(int msgType, PortCommunicator* comm = nullptr, LV2_URID dKey = DefinitionHandler::getInstance()->msg_key,
			double val = 0, double mi = 0, double ma = 1);
	virtual ~DoubleControl();

	virtual void receive(LV2_Atom_Object* data);

	double getValue();
	double getMin();
	double getMax();

	void setValue(double val);
	void setMin(double m);
	void setMax(double m);

private:
	double value;
	double min;
	double max;

	LV2_URID dataKey;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_DOUBLECONTROL_H_ */
