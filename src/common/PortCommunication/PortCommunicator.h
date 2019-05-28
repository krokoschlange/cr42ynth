/*
 * PortCommunicator.h
 *
 *  Created on: 28.05.2019
 *      Author: fabian
 */

#ifndef SRC_COMMON_PORTCOMMUNICATOR_H_
#define SRC_COMMON_PORTCOMMUNICATOR_H_

#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>
#include <vector>

#include "MessageReceiver.h"

namespace cr42y
{

class MessageReceiver;

class PortCommunicator
{
public:
	PortCommunicator();
	virtual ~PortCommunicator();

	virtual void receiveEvents() = 0;
	virtual void sendEvent(int msgType, LV2_Atom* data) = 0;

	void addReceiver(MessageReceiver* receiver);
	void removeReceiver(MessageReceiver* receiver);

private:
	std::vector<MessageReceiver*> receivers;

	LV2_URID_Map* map;

};

} /* namespace cr42y */

#endif /* SRC_COMMON_PORTCOMMUNICATOR_H_ */
