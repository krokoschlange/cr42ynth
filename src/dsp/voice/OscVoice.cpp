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

#include "OscVoice.h"
#include "../common.h"

namespace cr42y
{

OscVoice::OscVoice(WTOscillator* osc, float phase, float n, float vol,
		float wtp, float p) :
		oscillator(osc), lastPos(phase), deltaPhaseLFO_ENV(0), note(n), deltaFrequencyLFO_ENV(
				0), volume(vol), wtPos(wtp), pan(p), value(0), output(0), FM(1), RM(
				1), AM(1), PM(0)
{
	controls = new Control*[6];
	controls[0] = new Control(&volume, oscillator->getID() * 6 + 2);
	controls[1] = new Control(&deltaFrequencyLFO_ENV,
			oscillator->getID() * 6 + 3);
	controls[2] = new Control(&pan, oscillator->getID() * 6 + 4);
	controls[3] = new Control(&wtPos, oscillator->getID() * 6 + 5);
	//controls[4] = new Control(&volume, OSC_1_VOL); //TODO: Unison Controls
	//controls[5] = new Control(&volume, OSC_1_VOL);
}

OscVoice::~OscVoice()
{
	delete[] controls;
}

float OscVoice::getWTPos()
{
	return wtPos;
}

float OscVoice::getFrequency()
{
	return (note + deltaFrequencyLFO_ENV) * FM;
}

float OscVoice::getPhase()
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

void OscVoice::movePhase(float newPhase)
{
	if (newPhase > 1)
	{
		newPhase -= (int) newPhase;
	}
	lastPos = newPhase;
}

void OscVoice::nextSample()
{
	output = oscillator->getSample(this);
}

} /* namespace cr42y */
