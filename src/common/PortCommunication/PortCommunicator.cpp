/*
 * PortCommunicator.cpp
 *
 *  Created on: 28.05.2019
 *      Author: fabian
 */

#include "PortCommunicator.h"

namespace cr42y
{

PortCommunicator::PortCommunicator()
{

}

PortCommunicator::~PortCommunicator()
{
}

void PortCommunicator::addReceiver(MessageReceiver* receiver)
{
	for (int i = 0; i < receivers.size(); i++)
	{
		if (receivers[i] == receiver)
		{
			return;
		}
	}
	receivers.push_back(receiver);
}

void PortCommunicator::removeReceiver(MessageReceiver* receiver)
{
	for (int i = 0; i < receivers.size(); i++)
	{
		if (receivers[i] == receiver)
		{
			receivers.erase(receivers.begin() + i);
			i--;
		}
	}
}

} /* namespace cr42y */
