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

#include "Voice.h"
#include "CR42Ynth.h"
#include "Generators/WTOscillator.h"
#include "Controllers/OscillatorController.h"
#include "Controllers/EnvelopeController.h"
#include "Controllers/LFOController.h"

namespace cr42y
{

Voice::Voice(float n) :
		note(n)
{
	WTOscillator** oscs = CR42Ynth::getInstance()->getOscillators();
	int amount = 0;
	for (; oscs[amount]; amount++)
	{
	}
	oscillators = new OscillatorController*[amount + 1];
	for (int i = 0; i <= amount; i++)
	{
		oscillators[i] = new OscillatorController(this, oscs[i]);
	}
	
	Envelope** envs = CR42Ynth::getInstance()->getEnvelopes();
	amount = 0;
	for (; envs[amount]; amount++)
	{
	}
	envelopes = new EnvelopeController*[amount + 1];
	for (int i = 0; i <= amount; i++)
	{
		envelopes[i] = new EnvelopeController(this, envs[i]);
	}
	
	LFO** ls = CR42Ynth::getInstance()->getLFOs();
	amount = 0;
	for (; ls[amount]; amount++)
	{
	}
	lfos = new LFOController*[amount + 1];
	for (int i = 0; i <= amount; i++)
	{
		lfos[i] = new LFOController(this, ls[i]);
	}
}

Voice::~Voice()
{
	delete[] lfos;
	delete[] envelopes;
	delete[] oscillators;
}

float Voice::nextFrame()
{
	
}

float Voice::getNote()
{
	return note;
}

bool Voice::getSustain()
{
	return sustain;
}

} /* namespace cr42y */
