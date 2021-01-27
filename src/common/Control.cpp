/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
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

#include <cstring>

#include "rtosc/rtosc.h"

#include "Control.h"
#include "CR42YnthCommunicator.h"
#include "ControlListener.h"
#include "OSCEvent.h"

namespace cr42y
{

Control::Control(std::string addr, CR42YnthCommunicator* comm, float val,
		float mi, float ma, std::string gen) :
		address(addr),
		communicator(comm),
		value(val),
		generator(gen),
		max(ma),
		min(mi)
{
	//CR42YnthDSP::getInstance()->getCommunicator()->log(addr.c_str());
	//CR42YnthDSP::getInstance()->addControl(this);
	if (comm)
	{
		comm->addOSCEventListener(this);
	}
}

Control::~Control()
{
	//CR42YnthDSP::getInstance()->removeControl(this);
	if (communicator)
	{
		communicator->removeOSCEventListener(this);
	}
}

void Control::setValue(float val, bool callback, bool updateListeners)
{
	value = val;
	if (callback && communicator)
	{
		std::vector<OSCEvent> events;
		getState(events, true, false, false, false);
		communicator->writeMessage(events[0]);//sendState(true, false, false, false);
	}
	if (updateListeners)
	{
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->valueCallback(getValue(), this);
		}
	}
}

void Control::setMin(float m, bool callback, bool updateListeners)
{
	min = m;
	if (callback && communicator)
	{
		std::vector<OSCEvent> events;
		getState(events, false, true, false, false);
		communicator->writeMessage(events[0]);
	}
	if (updateListeners)
	{
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->minCallback(getMin(), this);
		}
	}
}

void Control::setMax(float m, bool callback, bool updateListeners)
{
	max = m;
	if (callback && communicator)
	{
		std::vector<OSCEvent> events;
		getState(events, false, false, true, false);
		communicator->writeMessage(events[0]);
	}
	if (updateListeners)
	{
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->maxCallback(getMax(), this);
		}
	}
}

void Control::setGenerator(std::string gen, bool callback, bool updateListeners)
{
	generator = gen;
	if (callback && communicator)
	{
		std::vector<OSCEvent> events;
		getState(events, false, false, false, true);
		communicator->writeMessage(events[0]);
	}
	if (updateListeners)
	{
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->genCallback(getGenerator(), this);
		}
	}
}

std::string Control::getAddress()
{
	return address;
}

std::string Control::getGenerator()
{
	return generator;
}

float Control::getMax()
{
	return max;
}

float Control::getMin()
{
	return min;
}

float Control::getValue()
{
	return value;
}

bool Control::handleOSCEvent(OSCEvent* event)
{
	const char* msg = event->getMessage();

	char* end = nullptr;
	rtosc_match_path(getAddress().c_str(), msg, (const char**) &end);
	if (end && *end == '\0')
	{
		if (rtosc_type(msg, 0) == 's')
		{
			if (!strcmp(rtosc_argument(msg, 0).s, "set_value"))
			{
				if (rtosc_type(msg, 1) == 'f')
				{
					setValue(rtosc_argument(msg, 1).f, false);
					return true;
				}
			}
			if (!strcmp(rtosc_argument(msg, 0).s, "set_min"))
			{
				if (rtosc_type(msg, 1) == 'f')
				{
					setMin(rtosc_argument(msg, 1).f, false);
					return true;
				}
			}
			if (!strcmp(rtosc_argument(msg, 0).s, "set_max"))
			{
				if (rtosc_type(msg, 1) == 'f')
				{
					setMax(rtosc_argument(msg, 1).f, false);
					return true;
				}
			}
			if (!strcmp(rtosc_argument(msg, 0).s, "set_generator"))
			{
				if (rtosc_type(msg, 1) == 's')
				{
					setGenerator(std::string(rtosc_argument(msg, 1).s), false);
					return true;
				}
			}
		}
	}
	return false;
}

void Control::getState(std::vector<OSCEvent>& events)
{
	getState(events, true, true, true, true);
}

void Control::getState(std::vector<OSCEvent>& events, bool sendVal, bool sendMin, bool sendMax, bool sendGen)
{
	if (!communicator)
	{
		return;
	}
	unsigned int bufferSize = getAddress().size() + 32;
	char buffer[bufferSize];
	if (sendVal)
	{
		rtosc_message(buffer, bufferSize, getAddress().c_str(), "sf",
				"set_value", getValue());
		events.push_back(OSCEvent(buffer, bufferSize, nullptr, 0));
		//communicator->writeMessage(buffer, len, nullptr, 0);
		//communicator->log(buffer);
	}
	if (sendMin)
	{
		rtosc_message(buffer, bufferSize, getAddress().c_str(), "sf",
				"set_min", getMin());
		events.push_back(OSCEvent(buffer, bufferSize, nullptr, 0));
		//communicator->writeMessage(buffer, len, nullptr, 0);
	}
	if (sendMax)
	{
		rtosc_message(buffer, bufferSize, getAddress().c_str(), "sf",
				"set_max", getMax());
		events.push_back(OSCEvent(buffer, bufferSize, nullptr, 0));
		//communicator->writeMessage(buffer, len, nullptr, 0);
	}
	if (sendGen)
	{
		rtosc_message(buffer, bufferSize, getAddress().c_str(), "ss",
				"set_generator", getGenerator().c_str());
		events.push_back(OSCEvent(buffer, bufferSize, nullptr, 0));
		//communicator->writeMessage(buffer, len, nullptr, 0);
	}
}

void Control::addListener(ControlListener* listener)
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == listener)
		{
			return;
		}
	}
	listeners.push_back(listener);
}

void Control::removeListener(ControlListener* listener)
{
	for (size_t i = 0; i < listeners.size(); i++)
	{
		if (listeners[i] == listener)
		{
			listeners.erase(listeners.begin() + i);
			i--;
		}
	}
}

} /* namespace cr42y */
