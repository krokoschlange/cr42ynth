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

#include "OscillatorController.h"
#include "../CR42Ynth.h"

namespace cr42y
{

OscillatorController::OscillatorController(Voice* v, WTOscillator* osc,
		int unisonNumber, bool isParent) :
		Controller(v),
		oscillator(osc),
		wtPos(0, CR42Ynth::getInstance()->getControlPort(), 0, 0, 1), //TODO
		volume(0, CR42Ynth::getInstance()->getControlPort(), 0, 0, 1),
		pan(0, CR42Ynth::getInstance()->getControlPort(), 0, -1, 1),
		frequencyShift(0, CR42Ynth::getInstance()->getControlPort(), 0, -4, 4),
		wavePos(0)

{
	if (isParent && osc->getUnisonVoices() > 1)
	{
		unison = new OscillatorController*[osc->getUnisonVoices() - 1];
		for (int i = 0; i < osc->getUnisonVoices() - 1; i++)
		{
			unison[i] = new OscillatorController(v, osc, i + 1);
		}
	}
	else
	{
		unison = nullptr;
	}

	if (osc->getUnisonVoices() > 1)
	{
		unisonSpreadFactor =
				-osc->getUnisonSpread() + unisonNumber * ((2 * osc->getUnisonSpread()) / osc->getUnisonVoices() - 1);
	}
	else
	{
		unisonSpreadFactor = 0;
	}
}

OscillatorController::~OscillatorController()
{
	for (int i = 0; unison[i]; i++)
	{
		delete unison[i];
	}
}

void OscillatorController::nextFrame()
{
	int oscAmount = 0;
	for (; unison && unison[oscAmount]; oscAmount++)
	{
		unison[oscAmount]->setFrequencyShift(
				oscillator->getUnisonDetune() * getFrequencyShift());
		unison[oscAmount]->setPan(
				getPan() + oscillator->getUnisonSpread() * unison[oscAmount]->getUnisonSpreadFactor());
	}
	float outs[oscAmount + 1];
	outs[0] = oscillator->getSample(&wavePos, getWTPos(), voice->getNote(),
			getFrequencyShift(), 0);
	for (int i = 0; i < oscAmount; i++)
	{
		unison[i]->nextFrame();
		outs[i + 1] = unison[i]->getValue();
	}
	float sum = 0;
	for (int i = 0; outs[i]; i++)
	{
		sum += outs[i];
	}
	*value = sum / (oscAmount + 1);
}

void OscillatorController::setWTPos(float pos)
{
	wtPos.setValue(pos);
}

void OscillatorController::setVolume(float vol)
{
	volume.setValue(vol);
}

void OscillatorController::setPan(float p)
{
	pan.setValue(p);
}

void OscillatorController::setFrequencyShift(float fShift)
{
	frequencyShift.setValue(fShift);
}

float OscillatorController::getWTPos()
{
	return wtPos.getValue();
}

float OscillatorController::getVolume()
{
	return volume.getValue();
}

float OscillatorController::getPan()
{
	return pan.getValue();
}

float OscillatorController::getFrequencyShift()
{
	return frequencyShift.getValue();
}

float OscillatorController::getUnisonSpreadFactor()
{
	return unisonSpreadFactor;
}

} /* namespace cr42y */
