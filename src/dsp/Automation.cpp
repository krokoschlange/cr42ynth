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

#include "Automation.h"

#include "CR42YnthDSP.h"
#include "AutomationData.h"

#include <cstring>

namespace cr42y
{

Automation::Automation(uint32_t id, CR42YnthCommunicator* comm, float smplrt) :
		ControlListener(),
		id_(id),
		waveform_(nullptr),
		wfSize_(0),
		samplerate_(smplrt),
		deltaPhase_(0),
		sustain_(1),
		data_(nullptr),
		communicator_(comm),
		typeControl_("/automation/" + std::to_string(id) + "/typeControl", comm, 0),
		syncControl_("/automation/" + std::to_string(id) + "/syncControl", comm, 0),
		useBeatsControl_("/automation/" + std::to_string(id) + "/useBeatsLength", comm, 0),
		secondsControl_("/automation/" + std::to_string(id) + "/lengthSeconds", comm, 0),
		beatsNumeratorControl_("/automation/" + std::to_string(id) + "/lengthBeatsNumerator", comm, 0),
		beatsDenominatorControl_("/automation/" + std::to_string(id) + "/lengthBeatsDenominator", comm, 0),
		sustainControl_("/automation/" + std::to_string(id) + "/sustainControl", comm, 0)
{
	connect(typeControl_);
	connect(syncControl_);
	connect(useBeatsControl_);
	connect(secondsControl_);
	connect(beatsNumeratorControl_);
	connect(beatsDenominatorControl_);
	connect(sustainControl_);
}

Automation::~Automation()
{
	if (data_)
	{
		delete data_;
	}
}

void Automation::valueCallback(float, Control* control)
{
	if (control == &syncControl_ || control == &useBeatsControl_ ||
		control == &secondsControl_ || control == &beatsNumeratorControl_ ||
		control == &beatsDenominatorControl_)
	{
		updateTiming();
	}
	else if (control == &sustainControl_)
	{
		updateSustain();
	}
}

void Automation::minCallback(float, Control*)
{
	
}

void Automation::maxCallback(float, Control*)
{
	
}

void Automation::genCallback(uint32_t, Control*)
{
	
}

void Automation::setData(AutomationData* data)
{
	if (data_)
	{
		delete data_;
	}
	data_ = data;
	std::vector<float> smpls;
	data->getSamples(smpls, 400);
	float* wf = new float[400];
	memcpy(wf, smpls.data(), 400 * sizeof(float));
	setWaveform(wf, 400);
	updateSustain();
}

void Automation::setWaveform(float* waveform, size_t size)
{
	if (waveform_)
	{
		delete[] waveform_;
	}
	waveform_ = waveform;
	wfSize_ = size;
}

void Automation::updateTiming()
{
	if (useBeatsControl_.getValue() > 0.5)
	{
		double beats = beatsNumeratorControl_.getValue() / beatsDenominatorControl_.getValue() * 4;
		double bps = CR42YnthDSP::getInstance()->getBPM() / 60;
		double seconds = beats / bps;
		deltaPhase_ = 1 / (seconds * samplerate_);
		
	}
	else
	{
		deltaPhase_ = 1 / (secondsControl_.getValue() * samplerate_);
	}
}

void Automation::updateSustain()
{
	if (data_)
	{
		uint32_t sustainPoint = sustainControl_.getValue();
		uint32_t sectionCount = data_->getSections().size();
		if (sectionCount > 0)
		{
			if (sustainPoint >= sectionCount)
			{
				sustainPoint = data_->getSections().size() - 1;
			}
			sustain_ = data_->getSections()[sustainPoint].start;
		}
		else
		{
			sustain_ = 1;
		}
	}
	else
	{
		sustain_ = 1;
	}
}

}
