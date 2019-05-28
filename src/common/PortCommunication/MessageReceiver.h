/*
 * MessageReceiver.h
 *
 *  Created on: 28.05.2019
 *      Author: fabian
 */

#ifndef SRC_COMMON_PORTCOMMUNICATION_MESSAGERECEIVER_H_
#define SRC_COMMON_PORTCOMMUNICATION_MESSAGERECEIVER_H_

#include <lv2/atom/atom.h>

#include "PortCommunicator.h"

namespace cr42y
{
class PortCommunicator;

class MessageReceiver
{
public:
	MessageReceiver(int msgType, PortCommunicator* comm = nullptr);
	virtual ~MessageReceiver();

	int getMessageType();

	void setMessageType(int msgType);

	virtual void receive(LV2_Atom_Object* data) = 0;

	void connect(PortCommunicator* comm);
	void disconnect();

private:
	PortCommunicator* communicator;
	int messageType;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_PORTCOMMUNICATION_MESSAGERECEIVER_H_ */
