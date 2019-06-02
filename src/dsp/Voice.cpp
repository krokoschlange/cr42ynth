/*
 * Voice.cpp
 *
 *  Created on: 02.06.2019
 *      Author: fabian
 */

#include "Voice.h"
#include "CR42Ynth.h"
#include "Generators/WTOscillator.h"

namespace cr42y
{

Voice::Voice()
{
	WTOscillator** oscs = CR42Ynth::getInstance()->getOscillators();
	int amount = 0;
	for (; oscs[amount]; amount++)
	{
	}
	oscillators = new OscillatorController*[amount + 1];
	for (int i = 0; i <= amount; i++)
	{
		oscillators[i] = new OscillatorController();
	}

	Envelope** envs = CR42Ynth::getInstance()->getEnvelopes();
	amount = 0;
	for (; envs[amount]; amount++)
	{
	}
	envelopes = new EnvelopeController*[amount + 1];
	for (int i = 0; i <= amount; i++)
	{
		envelopes[i] = new EnvelopeController();
	}

	LFO** ls = CR42Ynth::getInstance()->getLFOs();
	amount = 0;
	for (; ls[amount]; amount++)
	{
	}
	lfos = new LFOController*[amount + 1];
	for (int i = 0; i <= amount; i++)
	{
		lfos[i] = new LFOController(ls[i]);
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

} /* namespace cr42y */
