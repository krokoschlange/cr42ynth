/*******************************************************************************
 * Copyright (c) 2019 krokoschlange and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 * 3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior
 *     written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "MessageReceiver.h"

namespace cr42y
{

MessageReceiver::MessageReceiver(int msgType, PortCommunicator* comm, std::function<bool(LV2_Atom_Object*)>* checkerFunc) :
		messageType(msgType),
		communicator(comm),
		checkerFunction(checkerFunc)
{
	if (communicator)
	{
		communicator->addReceiver(this);
	}
}

MessageReceiver::~MessageReceiver()
{
	disconnect();
	delete checkerFunction;
}

int MessageReceiver::getMessageType()
{
	return messageType;
}

std::function<bool(LV2_Atom_Object*)>* MessageReceiver::getCheckerFunction()
{
	return checkerFunction;
}

void MessageReceiver::setMessageType(int msgType)
{
	messageType = msgType;
}

void MessageReceiver::setCheckerFunction(std::function<bool(LV2_Atom_Object*)>* checkerFunc)
{
	if (checkerFunction)
	{
		delete checkerFunction;
	}
	checkerFunction = checkerFunc;
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
