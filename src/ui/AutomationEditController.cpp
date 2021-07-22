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

#include "AutomationEditController.h"

#include "AutomationData.h"
#include "CR42YnthCommunicator.h"
#include "Control.h"
#include "OSCEvent.h"

#include <cmath>
#include "rtosc/rtosc.h"

namespace cr42y
{

AutomationEditController::AutomationEditController(CR42YnthCommunicator* communicator) :
		communicator_(communicator)
{
	createAutomation();
}

AutomationEditController::~AutomationEditController()
{
	for (size_t i = 0; i < automations_.size(); i++)
	{
		deleteAutomation(automations_[i]);
	}
}

void AutomationEditController::samples(std::vector<float>& samples, size_t amount)
{
	if (selected_ < automations_.size())
	{
		samples.clear();
		automations_[selected_].data_->getSamples(samples, amount);
	}
}

size_t AutomationEditController::sectionAmount()
{
	if (selected_ < automations_.size())
	{
		return automations_[selected_].data_->getSections().size();
	}
	return 0;
}

float AutomationEditController::handle(size_t section)
{
	if (selected_ < automations_.size())
	{
		return automations_[selected_].data_->getSections()[section].handle;
	}
	return -1;
}

float AutomationEditController::handlePosition(size_t section)
{
	if (selected_ < automations_.size())
	{
		return automations_[selected_].data_->getSectionSample(section, 0.5);
	}
	return -1;
}

void AutomationEditController::sectionHandle(float* x, float* y, size_t section)
{
	if (selected_ < automations_.size())
	{
		AutomationData::Section sec = automations_[selected_].data_->getSections()[section];
		if (x)
		{
			*x = sec.start;
		}
		if (y)
		{
			*y = sec.startValue;
		}
	}
}

void AutomationEditController::moveSection(size_t section, float x)
{
	if (selected_ < automations_.size())
	{
		automations_[selected_].data_->moveSection(section, x);
		dataChangedSignal_.emit();
	}
}

void AutomationEditController::moveSectionHandle(size_t section, float y)
{
	if (selected_ < automations_.size() && 0 <= y && y <= 1)
	{
		if (section > 0)
		{
			float start1 = automations_[selected_].data_->getSections()[section - 1].startValue;
			float oldStart2 = automations_[selected_].data_->getSections()[section].startValue;
			float oldHandle = automations_[selected_].data_->getSections()[section - 1].handle;
			if ((oldStart2 <= start1) != (y <= start1))
			{
				automations_[selected_].data_->moveHandle(section - 1, -oldHandle);
			}
		}
		if (section < automations_[selected_].data_->getSections().size())
		{
			float start1 = automations_[selected_].data_->getSections()[section].startValue;
			float oldStart2 = automations_[selected_].data_->getSections()[section + 1].startValue;
			float oldHandle = automations_[selected_].data_->getSections()[section].handle;
			if ((start1 <= oldStart2) != (y <= oldStart2))
			{
				automations_[selected_].data_->moveHandle(section, -oldHandle);
			}
		}
		automations_[selected_].data_->moveStartValue(section, y);
		dataChangedSignal_.emit();
	}
}

void AutomationEditController::moveHandle(size_t section, float y)
{
	if (selected_ < automations_.size())
	{
		automations_[selected_].data_->moveHandle(section, y);
		dataChangedSignal_.emit();
	}
}

size_t AutomationEditController::createSection(float x, float y)
{
	if (selected_ < automations_.size())
	{
		size_t index = automations_[selected_].data_->addSection({x, y, 0});
		dataChangedSignal_.emit();
		return index;
	}
	return 0;
}

void AutomationEditController::removeSection(size_t section)
{
	if (selected_ < automations_.size())
	{
		automations_[selected_].data_->removeSection(section);;
		dataChangedSignal_.emit();
	}
}

float AutomationEditController::getSectionHeight(size_t section)
{
	if (selected_ < automations_.size())
	{
		float y1 = 0;
		float y2 = 0;
		sectionHandle(nullptr, &y1, section);
		sectionHandle(nullptr, &y2, section + 1);
		return std::fabs(y1 - y2);
	}
	return 0;
}


size_t AutomationEditController::createAutomation()
{
	uint32_t id = 0;
	size_t index = 0;
	if (automations_.size() >= 2)
	{
		for (size_t i = 1; i < automations_.size(); i++)
		{
			if (automations_[i].id_ - automations_[i - 1].id_ > 1)
			{
				id = automations_[i - 1].id_ + 1;
				index = i;
			}
		}
		if (id == 0)
		{
			id = automations_[automations_.size() - 1].id_ + 1;
			index = automations_.size();
		}
	}
	else if (automations_.size() == 1)
	{
		if (automations_[0].id_ == 1)
		{
			id = 2;
			index = 1;
		}
		else
		{
			id = 1;
			index = 0;
		}
	}
	else
	{
		id = 1;
		index = 0;
	}
	char* buffer = new char[32];
	size_t len = rtosc_message(buffer, 32, "/automation/create", "i", id);
	OSCEvent createEvent(buffer, len, nullptr, 0);
	communicator_->writeMessage(createEvent);
	AutomationData* data = new AutomationData();
	Control* useBeatsLength = new Control("/automation/" + std::to_string(id) + "/useBeatsLength", communicator_, 1);
	Control* lengthSeconds = new Control("/automation/" + std::to_string(id) + "/lengthSeconds", communicator_, 0.25);
	Control* lengthBeatsNumerator = new Control("/automation/" + std::to_string(id) + "/lengthBeatsNumerator", communicator_, 1);
	Control* lengthBeatsDenominator = new Control("/automation/" + std::to_string(id) + "/lengthBeatsDenominator", communicator_, 4);
	Control* typeControl = new Control("/automation/" + std::to_string(id) + "/typeControl", communicator_, 0);
	Control* syncControl = new Control("/automation/" + std::to_string(id) + "/syncControl", communicator_, 0);
	Control* sustainControl = new Control("/automation/" + std::to_string(id) + "/sustainControl", communicator_, 0);
	automations_.insert(automations_.begin() + index,
						{
							id,
							data,
							useBeatsLength,
							lengthSeconds,
							lengthBeatsNumerator,
							lengthBeatsDenominator,
							typeControl,
							syncControl,
							sustainControl
						});
	selected_ = index;
	dataChangedSignal_.emit();
	selectionChangedSignal_.emit(selected_);
	return index;
}

void AutomationEditController::removeAutomation(size_t id)
{
	for (size_t i = 0; i < automations_.size(); i++)
	{
		if (automations_[i].id_ == id)
		{
			char* buffer = new char[32];
			size_t len = rtosc_message(buffer, 32, "/automation/remove", "i", id);
			OSCEvent deleteEvent(buffer, len, nullptr, 0);
			communicator_->writeMessage(deleteEvent);
			deleteAutomation(automations_[i]);
			automations_.erase(automations_.begin() + i);
			if (selected_ >= automations_.size())
			{
				selected_ = automations_.size() - 1;
				selectionChangedSignal_.emit(selected_);
			}
			dataChangedSignal_.emit();
			break;
		}
	}
}

void AutomationEditController::selectAutomation(size_t automation)
{
	if (automation < automations_.size())
	{
		selected_ = automation;
		selectionChangedSignal_.emit(selected_);
	}
}

size_t AutomationEditController::selectedAutomation()
{
	return selected_;
}

size_t AutomationEditController::getAutomationCount()
{
	return automations_.size();
}

int AutomationEditController::getType(size_t automation)
{
	if (automation < automations_.size())
	{
		return (int) automations_[automation].typeControl_->getValue();
	}
	return -1;
}

size_t AutomationEditController::getID(size_t automation)
{
	if (automation < automations_.size())
	{
		return automations_[automation].id_;
	}
	return 0;
}

Control* AutomationEditController::useBeatsLengthControl()
{
	return automations_[selected_].useBeatsLength_;
}

Control* AutomationEditController::lengthSecondsControl()
{
	return automations_[selected_].lengthSeconds_;
}

Control* AutomationEditController::lengthBeatsNumeratorControl()
{
	return automations_[selected_].lengthBeatsNumerator_;
}

Control* AutomationEditController::lengthBeatsDenominatorControl()
{
	return automations_[selected_].lengthBeatsDenominator_;
}

Control* AutomationEditController::typeControl()
{
	return automations_[selected_].typeControl_;
}

Control* AutomationEditController::syncControl()
{
	return automations_[selected_].syncControl_;
}

Control* AutomationEditController::sustainControl()
{
	return automations_[selected_].sustainControl_;
}

sigc::signal<void, size_t> & AutomationEditController::selectionChangedSignal()
{
	return selectionChangedSignal_;
}

sigc::signal<void> & AutomationEditController::dataChangedSignal()
{
	return dataChangedSignal_;
}

void AutomationEditController::deleteAutomation(Automation& data)
{
	delete data.data_;
	delete data.useBeatsLength_;
	delete data.lengthSeconds_;
	delete data.lengthBeatsNumerator_;
	delete data.lengthBeatsDenominator_;
	delete data.typeControl_;
	delete data.syncControl_;
	delete data.sustainControl_;
}

}
