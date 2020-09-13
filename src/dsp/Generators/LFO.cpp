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

#include <string>

#include "LFO.h"
#include "CR42YnthDSP.h"

namespace cr42y
{

LFO::LFO(std::vector<Voice*>* vce, int id, float rate) :
		Generator(vce),
		number(id),
		samplerate(rate),
		waveform(nullptr),
		syncedPhase(0), //TODO fix this
		unsyncedPhase(0),
		synced("/lfos/" + std::to_string(id) + "/sync", CR42YnthDSP::getInstance()->getCommunicator(), 1),
		retrigger("/lfos/" + std::to_string(id) + "/retrigger", CR42YnthDSP::getInstance()->getCommunicator(), 1),
		smooth("/lfos/" + std::to_string(id) + "/smooth", CR42YnthDSP::getInstance()->getCommunicator(), 1),
		useFrequency("/lfos/" + std::to_string(id) + "/use_freq", CR42YnthDSP::getInstance()->getCommunicator()),
		frequency("/lfos/" + std::to_string(id) + "/frequency", CR42YnthDSP::getInstance()->getCommunicator(), 1, 0, 10000),
		lenghtInNotes("/lfos/" + std::to_string(id) + "/lenght", CR42YnthDSP::getInstance()->getCommunicator(), 1, 0, 20)
{
	phases.insert(std::pair<Voice*, float>(CR42YnthDSP::getInstance()->getGlobalVoice(), 0));
	phases.insert(std::pair<Voice*, float>(CR42YnthDSP::getInstance()->getFreeVoice(), 0));
}

LFO::~LFO()
{
	if (waveform)
	{
		delete waveform;
	}
}

void LFO::nextSample()
{
	for (std::map<Voice*, float>::iterator it = phases.begin(); it != phases.end(); it++)
	{

	}
}

void LFO::voiceAdded(Voice* vce)
{
	phases.insert(std::pair<Voice*, float>(vce, 0));
}

void LFO::voiceRemoved(Voice* vce)
{
	std::map<Voice*, float>::iterator it = phases.find(vce);
	while (it != phases.end())
	{
		phases.erase(it);
		std::map<Voice*, float>::iterator it = phases.find(vce);
	}
}

void LFO::sendState()
{

}

bool LFO::receiveOSCMessage(OSCEvent* event)
{

	return false;
}

float LFO::getSample(Voice* vce)
{
	if (retrigger.getValue())
	{
		std::map<Voice*, float>::iterator it = values.find(vce);
		if (it != values.end())
		{
			return it->second;
		}
	}
	else
	{
		if (synced.getValue())
		{
			std::map<Voice*, float>::iterator it = values.find(CR42YnthDSP::getInstance()->getGlobalVoice());
			if (it != values.end())
			{
				return it->second;
			}
		}
		else
		{
			std::map<Voice*, float>::iterator it = values.find(CR42YnthDSP::getInstance()->getFreeVoice());
			if (it != values.end())
			{
				return it->second;
			}
		}
	}
	return 0;
}

void LFO::midiPanic()
{
	float glphase = 0;
	if (phases.find(CR42YnthDSP::getInstance()->getGlobalVoice()) != phases.end())
	{
		glphase = phases.find(CR42YnthDSP::getInstance()->getGlobalVoice())->second;
	}
	float fphase = 0;
	if (phases.find(CR42YnthDSP::getInstance()->getFreeVoice()) != phases.end())
	{
		fphase = phases.find(CR42YnthDSP::getInstance()->getFreeVoice())->second;
	}
	phases.clear();
	phases.insert(std::pair<Voice*, float>(CR42YnthDSP::getInstance()->getGlobalVoice(), glphase));
	phases.insert(std::pair<Voice*, float>(CR42YnthDSP::getInstance()->getFreeVoice(), fphase));
}

} /* namespace cr42y */
