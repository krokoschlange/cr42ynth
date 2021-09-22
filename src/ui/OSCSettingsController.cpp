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
#include "OSCSettingsController.h"

#include "common.h"
#include "WavetableEditData.h"
#include "OscillatorControls.h"
#include "CR42YnthCommunicator.h"
#include "OSCEvent.h"
#include "WavetableEditController.h"

#include "rtosc/rtosc.h"

#include <cstring>

namespace cr42y
{

OSCSettingsController::OSCSettingsController(CR42YnthCommunicator* communicator,
		WavetableEditController* wtEditController) :
		OSCEventListener(),
		communicator_(communicator),
		wtEditController_(wtEditController)
{
	editData_.resize(CR42Ynth_OSC_COUNT, WavetableEditData(4096));
	editHistory_.resize(CR42Ynth_OSC_COUNT,
			std::deque<std::pair<char*, std::vector<int>>>());

	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		controls_.push_back(new OscillatorControls(i, communicator));
	}

	if (communicator)
	{
		communicator->addOSCEventListener(this);
	}

	wtEditController->setData(&(editData_[0]), true);

	wtEditController->signalDataChanged().connect(
			sigc::mem_fun(this,
					&OSCSettingsController::wavetableChangedCallback));

	wtEditController->signalHistoryDelete().connect(
			sigc::mem_fun(this, &OSCSettingsController::eraseHistoryCallback));
}

OSCSettingsController::~OSCSettingsController()
{
	for (size_t i = 0; i < editHistory_.size(); i++)
	{
		for (size_t j = 0; j < editHistory_[i].size(); j++)
		{
			delete[] editHistory_[i][j].first;
		}
		editHistory_[i].clear();
	}
	editHistory_.clear();

	for (size_t i = 0; i < controls_.size(); i++)
	{
		delete controls_[i];
	}
	controls_.clear();

	if (communicator_)
	{
		communicator_->removeOSCEventListener(this);
	}
}

WavetableEditData* OSCSettingsController::getEditData(int idx)
{
	if (idx >= 0 && (size_t) idx < editData_.size())
	{
		return &(editData_[idx]);
	}
	return nullptr;
}

OscillatorControls* OSCSettingsController::getControls(int idx)
{
	if (idx >= 0 && (size_t) idx < controls_.size())
	{
		return controls_[idx];
	}
	return nullptr;
}

bool OSCSettingsController::handleOSCEvent(OSCEvent* event)
{
	const char* msg = event->getMessage();

	char* end = nullptr;
	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		rtosc_match_path(
				("/oscillators/" + std::to_string(i) + "/wavetable").c_str(),
				msg, (const char**) &end);
		if (end && *end == '\0')
		{
			if (rtosc_type(msg, 0) == 's'
					&& !strcmp(rtosc_argument(msg, 0).s, "set"))
			{
				if (event->getData())
				{
					editData_[i] = WavetableEditData((char*) event->getData());
				}
				else
				{
					editData_[i] = WavetableEditData(4096);
				}
				return true;
			}
			else if (rtosc_type(msg, 0) == 's'
					&& !strcmp(rtosc_argument(msg, 0).s, "update"))
			{
				if (event->getData())
				{
					editData_[i].update((char*) event->getData());
					return true;
				}
			}
		}
	}
	return false;
}

void OSCSettingsController::getState(std::vector<OSCEvent>& events)
{
	for (size_t i = 0; i < editData_.size(); i++)
	{
		std::string address = "/oscillators/" + std::to_string(i)
				+ "/wavetable";

		unsigned int bufferSize = address.size() + 32;
		char buffer[bufferSize];

		int len = rtosc_message(buffer, bufferSize, address.c_str(), "s",
				"set");

		uint8_t* dataBuffer = nullptr;
		int dataSize = editData_[i].getData((void**) &dataBuffer);
		events.push_back(OSCEvent(buffer, len, dataBuffer, dataSize));
		//communicator_->writeMessage(buffer, len, dataBuffer, dataSize);
		delete[] dataBuffer;
	}
}

void OSCSettingsController::wavetableChangedCallback()
{
	if (!communicator_)
	{
		return;
	}

	WavetableEditData* dataPtr = wtEditController_->data();

	int idx = -1;
	for (size_t i = 0; i < editData_.size() && idx < 0; i++)
	{
		if (dataPtr == &(editData_[i]))
		{
			idx = i;
		}
	}
	if (idx < 0)
	{
		return;
	}

	std::string address = "/oscillators/" + std::to_string(idx) + "/wavetable";

	unsigned int bufferSize = address.size() + 32;
	char buffer[bufferSize];

	int len = rtosc_message(buffer, bufferSize, address.c_str(), "s", "set");

	uint8_t* dataBuffer = nullptr;
	int dataSize = editData_[idx].getData((void**) &dataBuffer);
	OSCEvent event = OSCEvent(buffer, len, dataBuffer, dataSize);
	communicator_->writeMessage(event);
	delete[] dataBuffer;
}

void OSCSettingsController::eraseHistoryCallback(
		std::deque<std::pair<char*, std::vector<int>>>)
{

}

} /* namespace cr42y */
