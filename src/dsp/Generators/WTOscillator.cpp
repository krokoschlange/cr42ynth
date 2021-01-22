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

#include <cmath>
#include <cstring>

#include "rtosc/rtosc.h"

#include "WTOscillator.h"
#include "CR42YnthDSP.h"


#include "CR42YnthCommunicator.h"
#include "WavetableEditData.h"
#include "OscillatorVoiceData.h"
#include "OscillatorControls.h"
#include "OSCEvent.h"
#include "Voice.h"

namespace cr42y
{

WTOscillator::WTOscillator(CR42YnthCommunicator* comm, int id, float rate) :
		communicator_(comm),
		number(id),
		samplerate(rate),
		editData(new WavetableEditData(4096)),
		wavetable_(),
		controls_(new OscillatorControls(id, comm))
{
	std::vector<std::vector<float>> smpls;
	editData->getSamples(smpls);
	setWavetable(smpls);
	comm->addOSCEventListener(this);
}

WTOscillator::~WTOscillator()
{
	delete controls_;
	controls_ = nullptr;
}

void WTOscillator::setWavetable(std::vector<std::vector<float>>& wt)
{
	wavetable_ = wt;
}

std::vector<std::vector<float>>& WTOscillator::wavetable()
{
	return wavetable_;
}

void WTOscillator::setEditData(WavetableEditData* ed)
{
	if (editData)
	{
		delete editData;
	}
	editData = ed;
	//CR42YnthDSP::getInstance()->getCommunicator()->log(editData->to_string().c_str());
	if (editData && editData->getWaveforms()->size() > 0)
	{
		std::vector<std::vector<float>> smpls;
		editData->getSamples(smpls);
		setWavetable(smpls);
		/*std::string str = "";
		for (int i = 0; i < (*wavetable)[0].size(); i += 400)
		{
			str += std::to_string((*wavetable)[0][i]);
		}
		CR42YnthDSP::getInstance()->getCommunicator()->log(str.c_str());
		CR42YnthDSP::getInstance()->getCommunicator()->log(std::to_string(ed->getWidth()).c_str());*/
	}
	else
	{
		wavetable_.clear();
		wavetable_.push_back(std::vector<float>(2, 0));
	}
}

void WTOscillator::getState(std::vector<OSCEvent>& events)
{
	std::string address = "/oscillators/" + std::to_string(number) + "/wavetable";

	unsigned int bufferSize = address.size() + 32;
	char buffer[bufferSize];

	int len = rtosc_message(buffer, bufferSize, address.c_str(), "s", "set");

	if (editData)
	{
		/*float wtValues[width * height];
		 for (int i = 0; i < height; i++)
		 {
		 for (int j = 0; j < width; j++)
		 {
		 wtValues[i * width + j] = (*wavetable)[i][j];
		 }
		 }
		 CR42YnthDSP::getInstance()->getCommunicator()->writeMessage(buffer, len, (void*) wtValues, width * height * sizeof(float));
		 */
		void* dataBuffer = nullptr;
		int dataSize = editData->getData(&dataBuffer);
		events.push_back(OSCEvent(buffer, len, dataBuffer, dataSize));
		//communicator_->writeMessage(buffer, len, dataBuffer, dataSize);
	}
	else
	{
		events.push_back(OSCEvent(buffer, len, nullptr, 0));
		//communicator_->writeMessage(buffer, len, nullptr, 0);
	}
}

bool WTOscillator::handleOSCEvent(OSCEvent* event)
{
	const char* msg = event->getMessage();
	std::string pattern = "/oscillators/" + std::to_string(number) + "/wavetable";
	char* end = nullptr;
	rtosc_match_path(pattern.c_str(), msg, (const char**) &end);
	if (end && *end == '\0')
	{
		if (rtosc_type(msg, 0) == 's' && !strcmp(rtosc_argument(msg, 0).s, "set"))
		{
			if (event->getData())
			{
				setEditData(new WavetableEditData((char*) event->getData()));
			}
			else
			{
				setEditData(nullptr);
			}
			return true;
		}
		else if (rtosc_type(msg, 0) == 's' && !strcmp(rtosc_argument(msg, 0).s, "update"))
		{
			if (editData && event->getData())
			{
				editData->update((char*) event->getData());
				std::vector<std::vector<float>> smpls;
				editData->getSamples(smpls);
				setWavetable(smpls);
			}
		}
	}
	return false;
}

/*Control* WTOscillator::getActiveCtrl()
{
	return &active;
}

Control* WTOscillator::getSmoothCtrl()
{
	return &smooth;
}

Control* WTOscillator::getVolumeCtrl()
{
	return &volume;
}

Control* WTOscillator::getPanCtrl()
{
	return &pan;
}

Control* WTOscillator::getNoteShiftCtrl()
{
	return &noteShift;
}

Control* WTOscillator::getWTPosCtrl()
{
	return &wtPos;
}

Control* WTOscillator::getUnisonAmountCtrl()
{
	return &unisonAmount;
}

Control* WTOscillator::getUnisonDetuneCtrl()
{
	return &unisonDetune;
}

Control* WTOscillator::getUnisonSpreadCtrl()
{
	return &unisonSpread;
}

Control* WTOscillator::getPhaseShiftCtrl()
{
	return &phaseShift;
}

Control* WTOscillator::getPhaseRandCtrl()
{
	return &phaseRand;
}*/

OscillatorControls& WTOscillator::getControls()
{
	return *controls_;
}

} /* namespace cr42y */
