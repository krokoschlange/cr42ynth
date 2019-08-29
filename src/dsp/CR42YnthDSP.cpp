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

#include <cstring>

#include "rtosc.h"

#include "CR42YnthDSP.h"

namespace cr42y
{

CR42YnthDSP* CR42YnthDSP::instance;
CR42YnthDSP* CR42YnthDSP::getInstance(float rate, CR42YnthCommunicator* comm)
{
	if (!instance)
	{
		instance = new CR42YnthDSP(rate, comm);
	}
	return instance;
}

void CR42YnthDSP::destroyInstance()
{
	if (instance)
	{
		delete instance;
	}
	instance = nullptr;
}

CR42YnthDSP::CR42YnthDSP(float rate, CR42YnthCommunicator* comm) :
		globalVoice(0, 0),
		freeVoice(0, 0),
		samplerate(rate),
		communicator(comm),
		outL(nullptr),
		outR(nullptr),
		bpm(nullptr),
		bpmProp(nullptr),
		vol(nullptr),
		volProp(nullptr)
{
}

CR42YnthDSP::~CR42YnthDSP()
{
	if (outL)
	{
		delete[] outL;
	}
	outL = nullptr;
	if (outR)
	{
		delete[] outR;
	}
	outR = nullptr;
	for (int i = 0; i < oscillators.size(); i++)
	{
		delete oscillators[i];
	}
	oscillators.clear();
	if (bpm)
	{
		delete bpm;
		bpm = nullptr;
	}
	if (vol)
	{
		delete vol;
		vol = nullptr;
	}
	if (bpmProp)
	{
		delete bpmProp;
		bpmProp = nullptr;
	}
	if (volProp)
	{
		delete volProp;
		volProp = nullptr;
	}
	controls.clear();
}

void CR42YnthDSP::init()
{
	bpm = new Control("/global/bpm", getCommunicator(), 140, 0, 999);
	bpmProp = new Property(&globalVoice, bpm);

	vol = new Control("/global/volume", getCommunicator(), 1, 0, 1);
	volProp = new Property(&globalVoice, vol);

	for (int i = 0; i < 8; i++)
	{
		oscillators.push_back(new WTOscillator(&voices, i, samplerate));
	}
}

float CR42YnthDSP::getBPM()
{
	return bpm->getValue();
}

float CR42YnthDSP::getSamplerate()
{
	return samplerate;
}

CR42YnthCommunicator* CR42YnthDSP::getCommunicator()
{
	return communicator;
}

std::vector<WTOscillator*> CR42YnthDSP::getOscillators()
{
	return oscillators;
}

/*LFO** CR42YnthDSP::getLFOs()
 {
 return lfos;
 }

 Envelope** CR42YnthDSP::getEnvelopes()
 {
 return envelopes;
 }*/

float* CR42YnthDSP::getOutL()
{
	return outL;
}

float* CR42YnthDSP::getOutR()
{
	return outR;
}

void CR42YnthDSP::handleEvent(OSCEvent* event)
{
	bool eaten = false;

	char* end = nullptr;
	std::string pattern = "/global/midi";
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 'm')
	{
		uint8_t* midi = rtosc_argument(event->getMessage(), 0).m;
		uint8_t channel = midi[0] & 0b00001111; //TODO: do something with this
		uint8_t status = midi[0] >> 4;
		switch (status)
		{
		case 8: //Note off
			for (int i = 0; i < voices.size(); i++)
			{
				if (voices[i]->getNote() == midi[1])
				{
					for (int j = 0; j < oscillators.size(); j++)
					{
						oscillators[j]->voiceRemoved(voices[i]);
					}
					delete voices[i];
					voices.erase(voices.begin() + i);
					i--;
				}
			}
			break;
		case 9: //Note on
		{
			Voice* voice = new Voice(midi[1], midi[2]);
			voices.push_back(voice);
			for (int i = 0; i < oscillators.size(); i++)
			{
				oscillators[i]->voiceAdded(voice);
			}
			break;
		}
		case 11:
			if (midi[1] == 120 || midi[1] == 123) //All Sounds / Notes off
			{
				for (int i = 0; i < voices.size(); i++)
				{
					delete voices[i];
				}
				voices.clear();
				for (int i = 0; i < oscillators.size(); i++)
				{
					oscillators[i]->midiPanic();
				}
			}
			break;
		default:
			break;
		}
	}
	pattern = "/global/state";
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 's')
	{
		if (!strcmp(rtosc_argument(event->getMessage(), 0).s, "get"))
		{
			eaten = true;
			sendState();
		}
	}

	for (int i = 0; i < controls.size() && !eaten; i++)
	{
		eaten = controls[i]->receiveOSCMessage(event);
	}

	for (int i = 0; i < oscillators.size() && !eaten; i++)
	{
		eaten = oscillators[i]->receiveOSCMessage(event);
	}
}

void CR42YnthDSP::sendState()
{
	getCommunicator()->log("sending state");
	for (int i = 0; i < voices.size(); i++)
	{
		char buffer[32];
		int len = rtosc_message(buffer, 32, "/global/note", "i", voices[i]->getNote());
		getCommunicator()->writeMessage(buffer, len, nullptr, 0);
	}

	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->sendState();
	}
	for (int i = 0; i < oscillators.size(); i++)
	{
		oscillators[i]->sendState();
	}
}

void CR42YnthDSP::run(uint32_t n_samples)
{
	if (outL)
	{
		delete[] outL;
		outL = nullptr;
	}
	if (outR)
	{
		delete[] outR;
		outR = nullptr;
	}
	outL = new float[n_samples];
	outR = new float[n_samples];

	for (int s = 0; s < n_samples; s++)
	{
		outL[s] = 0;
		outR[s] = 0;
		for (int i = 0; i < oscillators.size(); i++)
		{
			if (oscillators[i]->getActiveCtrl()->getValue())
			{
				oscillators[i]->nextSample();
				for (int v = 0; v < voices.size(); v++)
				{
					std::vector<float> output = oscillators[i]->getOutput(voices[v]);
					if (output.size() > 1)
					{
						outL[s] += output[0];
						outR[s] += output[1];
					}
				}
			}
		}
	}
}

void CR42YnthDSP::addControl(Control* ctrl)
{
	for (int i = 0; i < controls.size(); i++)
	{
		if (controls[i] == ctrl)
		{
			return;
		}
	}
	controls.push_back(ctrl);
}

void CR42YnthDSP::removeControl(Control* ctrl)
{
	for (int i = 0; i < controls.size(); i++)
	{
		if (controls[i] == ctrl)
		{
			controls.erase(controls.begin() + i);
		}
	}
}

Generator* CR42YnthDSP::getGeneratorFromString(std::string str)
{
	if (str.size() == 0)
	{
		return nullptr;
	}
	if (str.compare(0, 3, std::string("osc")))
	{
		int number = std::stoi(str.substr(4));
		if (number < oscillators.size())
		{
			return oscillators[number];
		}
	}

	return nullptr;
}

Voice* CR42YnthDSP::getGlobalVoice()
{
	return &globalVoice;
}

Voice* CR42YnthDSP::getFreeVoice()
{
	return &freeVoice;
}

} /* namespace cr42y */
