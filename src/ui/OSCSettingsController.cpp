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
	controls_.reserve(CR42Ynth_OSC_COUNT);
	editHistory_.resize(CR42Ynth_OSC_COUNT,
			std::deque<std::pair<char*, std::vector<int>>>());

	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		controls_.push_back(OscillatorControls(i, communicator));
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
	for (int i = 0; i < editHistory_.size(); i++)
	{
		for (int j = 0; j < editHistory_[i].size(); j++)
		{
			delete[] editHistory_[i][j].first;
		}
		editHistory_[i].clear();
	}
	editHistory_.clear();

	if (communicator_)
	{
		communicator_->removeOSCEventListener(this);
	}
}

WavetableEditData* OSCSettingsController::getEditData(int idx)
{
	if (idx >= 0 && idx < editData_.size())
	{
		return &(editData_[idx]);
	}
	return nullptr;
}

OscillatorControls* OSCSettingsController::getControls(int idx)
{
	if (idx >= 0 && idx < controls_.size())
	{
		return &(controls_[idx]);
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

void OSCSettingsController::sendState()
{
	for (int i = 0; i < editData_.size(); i++)
	{
		std::string address = "/oscillators/" + std::to_string(i)
				+ "/wavetable";

		unsigned int bufferSize = address.size() + 32;
		char buffer[bufferSize];

		int len = rtosc_message(buffer, bufferSize, address.c_str(), "s",
				"set");

		void* dataBuffer = nullptr;
		int dataSize = editData_[i].getData(&dataBuffer);
		communicator_->writeMessage(buffer, len, dataBuffer, dataSize);
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
	for (int i = 0; i < editData_.size() && idx < 0; i++)
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

	void* dataBuffer = nullptr;
	int dataSize = editData_[idx].getData(&dataBuffer);
	communicator_->writeMessage(buffer, len, dataBuffer, dataSize);
}

void OSCSettingsController::eraseHistoryCallback(
		std::deque<std::pair<char*, std::vector<int>>>)
{

}

} /* namespace cr42y */
