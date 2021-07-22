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

#include "AutomationHandler.h"

#include "Automation.h"
#include "OSCEvent.h"

#include "rtosc/rtosc.h"

namespace cr42y
{

AutomationHandler::AutomationHandler(CR42YnthCommunicator* communicator) :
		communicator_(communicator)
{

}

AutomationHandler::~AutomationHandler()
{

}

bool AutomationHandler::handleOSCEvent(OSCEvent* event)
{
	char* end = nullptr;
	std::string pattern = "/automation/create";
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 'i')
	{
		createAutomation(rtosc_argument(event->getMessage(), 0).i);
		return true;
	}
	pattern = "/automation/remove";
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 'i')
	{
		removeAutomation(rtosc_argument(event->getMessage(), 0).i);
		return true;
	}
	return false;
}

void AutomationHandler::getState(std::vector<OSCEvent>&)
{
	
}

Automation* AutomationHandler::getAutomation(uint32_t id)
{
	if (automations_.size() > id)
	{
		return automations_[id];
	}
	return nullptr;
}

std::vector<Automation*>& AutomationHandler::getAutomations()
{
	return automations_;
}

void AutomationHandler::createAutomation(uint32_t id)
{
	if (automations_.size() <= id)
	{
		automations_.resize(id, nullptr);
	}
	automations_[id] = new Automation(id, communicator_);
}

void AutomationHandler::removeAutomation(uint32_t id)
{
	if (automations_.size() > id && automations_[id] != nullptr)
	{
		delete automations_[id];
		automations_[id] = nullptr;
	}
}

}
