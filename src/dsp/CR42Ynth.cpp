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

#include <lv2/core/lv2_util.h>

#include "CR42Ynth.h"

namespace cr42y
{

CR42Ynth* CR42Ynth::instance;
CR42Ynth::CR42Ynth(double rate, const char* bundlePath,
		const LV2_Feature* const * features) :
		bpm(140),
		samplerate(rate),
		ctrlIn(nullptr),
		ctrlOut(nullptr),
		midiIn(nullptr)
{
	const char* missing = lv2_features_query(features,
	LV2_URID__map, &map, true,
	NULL);

	control = new DSPPortCommunicator(ctrlIn, ctrlOut, map);

	int oscAmount = 8;
	oscillators = new WTOscillator*[oscAmount];
	for (int i = 0; i < oscAmount; i++)
	{
		oscillators[i] = new WTOscillator(rate, getControlPort());
	}
	for (int i = 0; i < oscAmount - 1; i++)
	{
		std::vector<ModulatableDoubleControl>* fmRow = new std::vector<
				ModulatableDoubleControl>;
		std::vector<ModulatableDoubleControl>* pmRow = new std::vector<
				ModulatableDoubleControl>;
		std::vector<ModulatableDoubleControl>* amRow = new std::vector<
				ModulatableDoubleControl>;
		std::vector<ModulatableDoubleControl>* rmRow = new std::vector<
				ModulatableDoubleControl>;
		fmControls.push_back(*fmRow);
		pmControls.push_back(*pmRow);
		amControls.push_back(*amRow);
		rmControls.push_back(*rmRow);
		for (int j = 0; j <= i; j++)
		{
			ModulatableDoubleControl* fmCtrl = new ModulatableDoubleControl(1,
					getControlPort(), 0, 0, 1); //TODO: msg_type & checker func
			ModulatableDoubleControl* pmCtrl = new ModulatableDoubleControl(1,
								getControlPort(), 0, 0, 1);
			ModulatableDoubleControl* amCtrl = new ModulatableDoubleControl(1,
								getControlPort(), 0, 0, 1);
			ModulatableDoubleControl* rmCtrl = new ModulatableDoubleControl(1,
								getControlPort(), 0, 0, 1);
			fmRow->push_back(*fmCtrl);
			pmRow->push_back(*pmCtrl);
			amRow->push_back(*amCtrl);
			rmRow->push_back(*rmCtrl);
		}
	}
}

CR42Ynth* CR42Ynth::getInstance(double rate, const char* bundlePath,
		const LV2_Feature* const * features)
{
	if (!instance)
	{
		instance = new CR42Ynth(rate, bundlePath, features);
	}
	return instance;
}

CR42Ynth::~CR42Ynth()
{
}

float CR42Ynth::getBPM()
{
	return bpm;
}

WTOscillator** CR42Ynth::getOscillators()
{
	return oscillators;
}

LFO** CR42Ynth::getLFOs()
{
	return lfos.data();
}

Envelope** CR42Ynth::getEnvelopes()
{
	return envelopes.data();
}

DSPPortCommunicator* CR42Ynth::getControlPort()
{
	return control;
}

DSPPortCommunicator* CR42Ynth::getInputPort()
{
	return input;
}

ExternalPort** CR42Ynth::getExternalPorts()
{
	return externalPorts;
}

std::vector<std::vector<ModulatableDoubleControl>>* CR42Ynth::getFMControls()
{
	return &fmControls;
}

std::vector<std::vector<ModulatableDoubleControl>>* CR42Ynth::getPMControls()
{
	return &pmControls;
}

std::vector<std::vector<ModulatableDoubleControl>>* CR42Ynth::getAMControls()
{
	return &amControls;
}

std::vector<std::vector<ModulatableDoubleControl>>* CR42Ynth::getRMControls()
{
	return &rmControls;
}

LV2_Handle CR42Ynth::lv2Instatiate(const LV2_Descriptor* descriptor,
		double samplerate, const char* bundlePath,
		const LV2_Feature* const * features)
{
	CR42Ynth* c = CR42Ynth::getInstance(samplerate, bundlePath, features);
	return (LV2_Handle) c;
}

void CR42Ynth::lv2Activate(LV2_Handle instance)
{

}

void CR42Ynth::lv2Deactivate(LV2_Handle instance)
{

}

void CR42Ynth::lv2ConnectPort(LV2_Handle instance, uint32_t port, void* data)
{

}

void CR42Ynth::lv2Run(LV2_Handle instance, uint32_t n_samples)
{

}

void CR42Ynth::lv2Cleanup(LV2_Handle instance)
{

}

const void* CR42Ynth::lv2ExtensionData(const char* uri)
{

}

} /* namespace cr42y */
