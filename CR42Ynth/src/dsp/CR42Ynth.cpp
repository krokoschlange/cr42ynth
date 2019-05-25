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

#include "CR42Ynth.h"
#include <cstring>

namespace cr42y
{
CR42Ynth* CR42Ynth::singleton;

CR42Ynth::CR42Ynth(double rate, const char* bundlePath,
		const LV2_Feature* const * features)
{
	for (int i = 0; features[i]; i++)
	{
		if (!strcmp(features[i]->URI, LV2_URID__map))
		{
			map = (LV2_URID_Map*) features[i]->data;
		}
	}
	if (map)
	{
		urids = CR42YnthURIS::getInstance(map);
	}
}

CR42Ynth::~CR42Ynth()
{
}

CR42Ynth* CR42Ynth::getInstance(double rate, const char* bundlePath,
		const LV2_Feature* const * features)
{
	if (singleton == 0)
	{
		singleton = new CR42Ynth(rate, bundlePath, features);
	}
	return singleton;
}

CR42Ynth* CR42Ynth::getInstance()
{
	return singleton;
}

void CR42Ynth::connectPort(uint32_t port, void* data)
{
	switch (port)
	{
	case CR42YNTH_MIDI:

		break;
	case CR42YNTH_OUT0:

		break;
	case CR42YNTH_OUT1:

		break;
	case CR42YNTH_CTRL:

		break;
	case CR42YNTH_NOTIFY:

		break;
	default:
		if (port >= 5 && port <= 12)
		{
			//ext port
		}
	}
}

void CR42Ynth::activate()
{

}

void CR42Ynth::run(uint32_t n_samples)
{

}

void CR42Ynth::deactivate()
{

}

LV2_Handle CR42Ynth::lv2Instantiate(const LV2_Descriptor* descriptor,
		double sampleRate, const char* bundlePath,
		const LV2_Feature* const * features)
{
	CR42Ynth* instance = getInstance(sampleRate, bundlePath, features);
	return (LV2_Handle) instance;
}

void CR42Ynth::lv2ConnectPort(LV2_Handle instance, uint32_t port, void* data)
{
	CR42Ynth* cr42ynth = (CR42Ynth*) instance;
	cr42ynth->connectPort(port, data);
}

void CR42Ynth::lv2Activate(LV2_Handle instance)
{
	CR42Ynth* cr42ynth = (CR42Ynth*) instance;
	cr42ynth->activate();
}

void CR42Ynth::lv2Run(LV2_Handle instance, uint32_t n_samples)
{
	CR42Ynth* cr42ynth = (CR42Ynth*) instance;
	cr42ynth->run(n_samples);
}

void CR42Ynth::lv2Deactivate(LV2_Handle instance)
{
	CR42Ynth* cr42ynth = (CR42Ynth*) instance;
	cr42ynth->deactivate();
}

void CR42Ynth::lv2Cleanup(LV2_Handle instance)
{
	CR42Ynth* cr42ynth = (CR42Ynth*) instance;
	delete cr42ynth;
}

const void* CR42Ynth::lv2ExtensionData(const char* uri)
{
	return nullptr;
}

void CR42Ynth::controlSubscribe(Control* ctrl)
{
	controls.push_back(ctrl);
}

void CR42Ynth::controlUnsubscribe(Control* ctrl)
{
	for (int i = 0; i < controls.size(); i++)
	{
		if (controls[i] == ctrl)
		{
			controls.erase(controls.begin() + i);
			i--;
		}
	}
}

} /* namespace cr42y */
