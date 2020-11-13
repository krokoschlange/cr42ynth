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

#include <iostream>
#include <lv2/core/lv2.h>
#include <lv2/state/state.h>
#include "CR42YnthLV2.h"
#include "common.h"

namespace cr42y
{
LV2_Handle instantiate(const LV2_Descriptor* descriptor, double samplerate,
		const char* bundlePath, const LV2_Feature* const * features)
{
	CR42YnthLV2* lv2 = CR42YnthLV2::getInstance(samplerate, bundlePath,
			features);
	if (lv2->isReady())
	{
		lv2->log("instantiated");
		return (LV2_Handle) lv2;
	}
	return nullptr;
}

void activate(LV2_Handle instance)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	lv2->log("activate");
	lv2->activate();
}

void deactivate(LV2_Handle instance)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	lv2->log("deactivate");
	lv2->deactivate();
}

void connectPort(LV2_Handle instance, uint32_t port, void* data)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	lv2->connectPort(port, data);
}

void run(LV2_Handle instance, uint32_t n_samples)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	lv2->run(n_samples);
}

void cleanup(LV2_Handle instance)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	lv2->log("cleanup");
	lv2->cleanup();
	delete lv2;
	lv2 = nullptr;
}

LV2_State_Status save(LV2_Handle instance, LV2_State_Store_Function store,
		LV2_State_Handle handle, uint32_t flags,
		const LV2_Feature* const * features)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	return lv2->save(store, handle, flags, features);
}

LV2_State_Status restore(LV2_Handle instance,
		LV2_State_Retrieve_Function retrieve, LV2_State_Handle handle,
		uint32_t flags, const LV2_Feature* const * features)
{
	CR42YnthLV2* lv2 = (CR42YnthLV2*) instance;
	return lv2->restore(retrieve, handle, flags, features);
}

static const LV2_State_Interface stateIface =
	{save, restore};

const void* extensionData(const char* uri)
{
	if (!strcmp(uri, LV2_STATE__interface))
	{
		return &stateIface;
	}
	return nullptr;
}

static const LV2_Descriptor descriptor =
	{
	CR42Ynth__URI, instantiate, connectPort, activate, run, deactivate, cleanup,
			extensionData};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
	{
	case 0:
		return &descriptor;
	default:
		return nullptr;
	}
}
} /* namespace cr42y */

int main()
{
	std::cout << "Use as LV2 Plugin";
	return 0;
}

