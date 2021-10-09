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
		samplerate_(rate),
		communicator_(comm),
		bpm_(nullptr),
		position_(nullptr),
		vol_(nullptr),
		modCtrls_(nullptr),
		automationHandler_(nullptr),
		outR_(nullptr),
		outL_(nullptr)
{
	comm->addOSCEventListener(this);
	modCtrls_ = new ModulationControls(comm);
	automationHandler_ = new AutomationHandler(comm, samplerate_);
}

CR42YnthDSP::~CR42YnthDSP()
{
	for (size_t i = 0; i < oscillators_.size(); i++)
	{
		delete oscillators_[i];
	}
	oscillators_.clear();
	if (bpm_)
	{
		delete bpm_;
		bpm_ = nullptr;
	}
	if (position_)
	{
		delete position_;
		position_ = nullptr;
	}
	if (vol_)
	{
		delete vol_;
		vol_ = nullptr;
	}

	delete modCtrls_;
}

void CR42YnthDSP::init()
{
	bpm_ = new Control("/global/bpm", getCommunicator(), 140, 0, 999);
	bpm_ = new Control("/global/beat", getCommunicator(), 0, 0, 999);

	vol_ = new Control("/global/volume", getCommunicator(), 1, 0, 1);

	for (int i = 0; i < CR42Ynth_OSC_COUNT; i++)
	{
		oscillators_.push_back(new WTOscillator(getCommunicator(), i, samplerate_));
	}
}

float CR42YnthDSP::getBPM()
{
	return bpm_->getValue();
}

float CR42YnthDSP::getSamplerate()
{
	return samplerate_;
}

CR42YnthCommunicator* CR42YnthDSP::getCommunicator()
{
	return communicator_;
}

std::vector<WTOscillator*> CR42YnthDSP::getOscillators()
{
	return oscillators_;
}

void CR42YnthDSP::setSink(float* left, float* right)
{
	outL_ = left;
	outR_ = right;
}

float* CR42YnthDSP::getOutL()
{
	return outL_;
}

float* CR42YnthDSP::getOutR()
{
	return outR_;
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
			for (size_t i = 0; i < voices_.size(); i++)
			{
				if (voices_[i]->getNote() == midi[1])
				{
					voices_[i]->stopPress();
				}
			}
			eaten = true;
			break;
		case 9: //Note on
		{
			std::vector<WTOscillator*> activeOscs;
			for (size_t i = 0; i < oscillators_.size(); i++)
			{
				if (oscillators_[i]->getControls().getActiveCtrl()->getValue())
				{
					activeOscs.push_back(oscillators_[i]);
				}
			}
			Voice* voice = new Voice(midi[1], midi[2], activeOscs, modCtrls_, automationHandler_);
			voices_.push_back(voice);
			eaten = true;
			break;
		}
		case 11:
			if (midi[1] == 120 || midi[1] == 123) //All Sounds / Notes off
			{
				for (size_t i = 0; i < voices_.size(); i++)
				{
					delete voices_[i];
				}
				voices_.clear();
			}
			eaten = true;
			break;
		default:
			break;
		}
	}
	return eaten;
}

void CR42YnthDSP::getState(std::vector<OSCEvent>& events)
{
	getCommunicator()->log("sending state");
	for (size_t i = 0; i < voices_.size(); i++)
	{
		char buffer[32];
		int len = rtosc_message(buffer, 32, "/global/note", "i", voices_[i]->getNote());
		events.push_back(OSCEvent(buffer, len, nullptr, 0));
	}
}

void CR42YnthDSP::run(uint32_t n_samples)
{
	std::fill(outL_, outL_ + n_samples, 0);
	std::fill(outR_, outR_ + n_samples, 0);

	for (size_t i = 0; i < voices_.size(); i++)
	{
		voices_[i]->calculate(outL_, outR_, n_samples);
	}
	for (size_t i = 0; i < voices_.size(); i++)
	{
		if (voices_[i]->hasEnded())
		{
			delete voices_[i];
			voices_.erase(voices_.begin() + i);
			i--;
		}
	}
}

} /* namespace cr42y */
