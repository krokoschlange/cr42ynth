/*
 * MessageReceiver.cpp
 *
 *  Created on: 28.05.2019
 *      Author: fabian
 */

#include "MessageReceiver.h"

namespace cr42y
{

MessageReceiver::MessageReceiver(int msgType, PortCommunicator* comm) :
		messageType(msgType),
		communicator(comm)
{
	if (communicator)
	{
		communicator->addReceiver(this);
	}
}

MessageReceiver::~MessageReceiver()
{
}

int MessageReceiver::getMessageType()
{
	return messageType;
}

void MessageReceiver::setMessageType(int msgType)
{
	messageType = msgType;
}

void MessageReceiver::connect(PortCommunicator* comm)
{
	if (communicator)
	{
		communicator->removeReceiver(this);
	}
	communicator = comm;
	communicator->addReceiver(this);
}

void MessageReceiver::disconnect()
{
	if (communicator)
	{
		communicator->removeReceiver(this);
		communicator = nullptr;
	}
}

} /* namespace cr42y */
