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

#include "CR42YnthDSP.h"

#include "OscillatorControls.h"

#include "CR42YnthCommunicator.h"
#include "OSCEvent.h"
#include "Voice.h"
#include "WTOscillator.h"
#include "Control.h"
#include "Property.h"
#include "common.h"
#include "ModulationControls.h"
#include "AutomationHandler.h"

#include <iostream>

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
		samplerate(rate),
		communicator(comm),
		bpm(nullptr),
		vol(nullptr),
		modCtrls_(nullptr),
		automationHandler_(nullptr),
		globalVoice(nullptr),//new Voice(0, 0, std::vector<WTOscillator*>())),
		freeVoice(nullptr),//new Voice(0, 0, std::vector<WTOscillator*>())),
		outR(nullptr),
		outL(nullptr)
{
	comm->addOSCEventListener(this);
	modCtrls_ = new ModulationControls(comm);
	automationHandler_ = new AutomationHandler(comm, samplerate);
}

CR42YnthDSP::~CR42YnthDSP()
{
	/*if (outL)
	{
		delete[] outL;
	}
	outL = nullptr;
	if (outR)
	{
		delete[] outR;
	}
	outR = nullptr;*/
	for (size_t i = 0; i < oscillators.size(); i++)
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

	delete globalVoice;
	globalVoice = nullptr;
	delete freeVoice;
	freeVoice = nullptr;
	delete modCtrls_;

	//controls.clear();
}

void CR42YnthDSP::init()
{
	bpm = new Control("/global/bpm", getCommunicator(), 140, 0, 999);

	vol = new Control("/global/volume", getCommunicator(), 1, 0, 1);

	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		oscillators.push_back(new WTOscillator(getCommunicator(), i, samplerate));
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

void CR42YnthDSP::setSink(float* left, float* right)
{
	outL = left;
	outR = right;
}

float* CR42YnthDSP::getOutL()
{
	return outL;
}

float* CR42YnthDSP::getOutR()
{
	return outR;
}

bool CR42YnthDSP::handleOSCEvent(OSCEvent* event)
{
	size_t msglen = 0;
	const unsigned char* msg = (const unsigned char*) event->getMessage(&msglen);
	for (size_t i = 0; i < msglen; i++)
	{
		if (msg[i] >= 32 && msg[i] < 127)
		{
			std::cout << msg[i];
		}
		else
		{
			std::cout << " 0x" << std::hex << (unsigned int) msg[i] << std::dec << " ";
		}
	}
	std::cout << "\n";
	
	bool eaten = false;
		
	char* end = nullptr;
	std::string pattern = "/global/midi";
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 'm')
	{
		rtosc_arg_t argument = rtosc_argument(event->getMessage(), 0);
		uint8_t* midi = argument.m;
		//uint8_t channel = midi[0] & 0b00001111; //TODO: do something with this
		uint8_t status = midi[0] >> 4;
				
		switch (status)
		{
		case 8: //Note off
			for (size_t i = 0; i < voices.size(); i++)
			{
				if (voices[i]->getNote() == midi[1])
				{
					/*for (int j = 0; j < oscillators.size(); j++)
					{
						oscillators[j]->voiceRemoved(voices[i]);
					}*/
					voices[i]->stopPress();
				}
			}
			eaten = true;
			break;
		case 9: //Note on
		{
			std::vector<WTOscillator*> activeOscs;
			for (size_t i = 0; i < oscillators.size(); i++)
			{
				if (oscillators[i]->getControls().getActiveCtrl()->getValue())
				{
					activeOscs.push_back(oscillators[i]);
				}
			}
			Voice* voice = new Voice(midi[1], midi[2], activeOscs, modCtrls_, automationHandler_);
			voices.push_back(voice);
			/*for (int i = 0; i < oscillators.size(); i++)
			{
				oscillators[i]->voiceAdded(voice);
			}*/
			eaten = true;
			break;
		}
		case 11:
			if (midi[1] == 120 || midi[1] == 123) //All Sounds / Notes off
			{
				for (size_t i = 0; i < voices.size(); i++)
				{
					delete voices[i];
				}
				voices.clear();
			}
			eaten = true;
			break;
		default:
			break;
		}
	}
	/*pattern = "/global/state";
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0' && rtosc_type(event->getMessage(), 0) == 's')
	{
		if (!strcmp(rtosc_argument(event->getMessage(), 0).s, "get"))
		{
			eaten = true;
			sendState();
		}
	}*/

	/*for (int i = 0; i < controls.size() && !eaten; i++)
	{
		eaten = controls[i]->receiveOSCMessage(event);
	}*/

	/*for (int i = 0; i < oscillators.size() && !eaten; i++)
	{
		eaten = oscillators[i]->receiveOSCMessage(event);
	}*/
	return eaten;
}

void CR42YnthDSP::getState(std::vector<OSCEvent>& events)
{
	getCommunicator()->log("sending state");
	for (size_t i = 0; i < voices.size(); i++)
	{
		char buffer[32];
		int len = rtosc_message(buffer, 32, "/global/note", "i", voices[i]->getNote());
		events.push_back(OSCEvent(buffer, len, nullptr, 0));
		//getCommunicator()->writeMessage(buffer, len, nullptr, 0);
	}

	/*for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->sendState();
	}*/
	/*for (int i = 0; i < oscillators.size(); i++)
	{
		oscillators[i]->sendState();
	}*/
}

void CR42YnthDSP::run(uint32_t n_samples)
{
	/*if (outL)
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
	outR = new float[n_samples];*/

	std::fill(outL, outL + n_samples, 0);
	std::fill(outR, outR + n_samples, 0);
	/*for (int i = 0; i < oscillators.size(); i++)
	{
		if (oscillators[i]->getControls().getActiveCtrl()->getValue())
		{
			for (int s = 0; s < n_samples; s++)
			{
				oscillators[i]->nextSample(outL + s, outR + s);
			}
		}
	}*/
	for (size_t i = 0; i < voices.size(); i++)
	{
		voices[i]->calculate(outL, outR, n_samples);
	}
	for (size_t i = 0; i < voices.size(); i++)
	{
		if (voices[i]->hasEnded())
		{
			delete voices[i];
			voices.erase(voices.begin() + i);
			i--;
		}
	}
}

/*void CR42YnthDSP::addControl(Control* ctrl)
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
}*/

void CR42YnthDSP::calculateOscillators(float*, float*, uint32_t samples)
{
	
	for (uint32_t i = 0; i < samples; i++)
	{
		
	}
}

Generator* CR42YnthDSP::getGeneratorFromString(std::string str)
{
	if (str.size() == 0)
	{
		return nullptr;
	}
	

	return nullptr;
}

Voice* CR42YnthDSP::getGlobalVoice()
{
	return globalVoice;
}

Voice* CR42YnthDSP::getFreeVoice()
{
	return freeVoice;
}

} /* namespace cr42y */
