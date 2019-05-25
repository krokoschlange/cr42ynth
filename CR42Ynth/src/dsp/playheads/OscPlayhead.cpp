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

#include "OscPlayhead.h"
#include "../common.h"

namespace cr42y
{

OscPlayhead::OscPlayhead(WTOscillator* osc, float phase, float n, float vol,
		float wtp, float p) :
		oscillator(osc), lastPos(phase), deltaPhaseLFO_ENV(0), note(n), deltaFrequencyLFO_ENV(
				0), volume(vol), wtPos(wtp), pan(p), value(0), output(0), FM(1), RM(
				1), AM(1), PM(0)
{
	controls = new Control*[6];
	;
	controls[0] = new Control(0, std::bind(&OscPlayhead::setVol, this, std::placeholders::_1), std::bind(&OscPlayhead::getVol, this));
	controls[1] = new Control(1, std::bind(&OscPlayhead::setDeltaFreq, this, std::placeholders::_1), std::bind(&OscPlayhead::getDeltaFreq, this));
	controls[2] = new Control(2, std::bind(&OscPlayhead::setDeltaPhase, this, std::placeholders::_1), std::bind(&OscPlayhead::getDeltaPhase, this));
	controls[3] = new Control(3, std::bind(&OscPlayhead::setPan, this, std::placeholders::_1), std::bind(&OscPlayhead::getPan, this));
	controls[4] = new Control(4, std::bind(&OscPlayhead::setWTPos, this, std::placeholders::_1), std::bind(&OscPlayhead::getWTPos, this));
}

OscPlayhead::~OscPlayhead()
{
	delete[] controls;
}

float OscPlayhead::getNote()
{
	return note;//pow(2, (note - 69) / 12) * 440 * deltaFrequencyLFO_ENV * FM;
}

float OscPlayhead::getPhase()
{
	float phase = lastPos + deltaPhaseLFO_ENV + PM;
	if (phase > 1)
	{
		phase -= (int) phase;
	}
	else if (phase < 0)
	{
		phase -= (int) phase;
		phase += 1;
	}
	return phase;
}

void OscPlayhead::movePhase(float newPhase)
{
	if (newPhase > 1)
	{
		newPhase -= (int) newPhase;
	}
	lastPos = newPhase;
}

void OscPlayhead::nextSample()
{
	output = oscillator->getSample(this);
}

void OscPlayhead::setDeltaPhase(float p)
{
	deltaPhaseLFO_ENV = p;
}

float OscPlayhead::getDeltaPhase()
{
	return deltaPhaseLFO_ENV;
}

void OscPlayhead::setDeltaFreq(float f)
{
	deltaFrequencyLFO_ENV = f;
}
float OscPlayhead::getDeltaFreq()
{
	return deltaFrequencyLFO_ENV;
}

void OscPlayhead::setVol(float v)
{
	volume = v;
}
float OscPlayhead::getVol()
{
	return volume;
}

void OscPlayhead::setWTPos(float pos)
{
	wtPos = pos;
}
float OscPlayhead::getWTPos()
{
	return wtPos;
}

void OscPlayhead::setPan(float p)
{
	pan = p;
}
float OscPlayhead::getPan()
{
	return pan;
}

void OscPlayhead::setFM(float f)
{
	FM = f;
}
float OscPlayhead::getFM()
{
	return FM;
}

void OscPlayhead::setAM(float a)
{
	AM = a;
}
float OscPlayhead::getAM()
{
	return AM;
}

void OscPlayhead::setPM(float p)
{
	PM = p;
}
float OscPlayhead::getPM()
{
	return PM;
}

void OscPlayhead::setRM(float r)
{
	RM = r;
}
float OscPlayhead::getRM()
{
	return RM;
}


} /* namespace cr42y */
