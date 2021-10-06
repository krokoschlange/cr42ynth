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

#include "CR42YnthCommunicator.h"

#include "OSCEvent.h"
#include "OSCEventListener.h"

#include <string>
#include <string.h>
#include "rtosc/rtosc.h"

namespace cr42y
{

CR42YnthCommunicator::CR42YnthCommunicator()
{
}

CR42YnthCommunicator::~CR42YnthCommunicator()
{
}

void CR42YnthCommunicator::addOSCEventListener(OSCEventListener* listener)
{
	listeners_.push_back(listener);
}

void CR42YnthCommunicator::removeOSCEventListener(OSCEventListener* listener)
{
	for (unsigned int i = 0; i < listeners_.size(); i++)
	{
		if (listener == listeners_[i])
		{
			listeners_.erase(listeners_.begin() + i);
			i--;
		}
	}
}

bool CR42YnthCommunicator::handleOSCEvent(OSCEvent* event)
{
	bool handled = false;

	std::string pattern = "/global/state";
	char* end = nullptr;
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 's')
	{
		if (!strcmp(rtosc_argument(event->getMessage(), 0).s, "get"))
		{
			handled = true;
			sendState();
		}
	}
	for (size_t i = 0; i < listeners_.size() && !handled; i++)
	{
		handled = listeners_[i]->handleOSCEvent(event);
	}
	return handled;
}

void CR42YnthCommunicator::sendState()
{
	std::vector<OSCEvent> events;
	for (size_t i = 0; i < listeners_.size(); i++)
	{
		if (listeners_[i]->hasPriority())
		{
			listeners_[i]->getState(events);
			for (unsigned int j = 0; j < events.size(); j++)
			{
				writeMessage(events[j]);
			}
			events.clear();
		}
	}
	for (size_t i = 0; i < listeners_.size(); i++)
	{
		if (!listeners_[i]->hasPriority())
		{
			listeners_[i]->getState(events);
			for (unsigned int j = 0; j < events.size(); j++)
			{
				writeMessage(events[j]);
			}
			events.clear();
		}
	}
}

} /* namespace cr42y */
