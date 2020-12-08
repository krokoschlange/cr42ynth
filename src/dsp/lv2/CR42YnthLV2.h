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

#ifndef SRC_DSP_LV2_CR42YNTHLV2_H_
#define SRC_DSP_LV2_CR42YNTHLV2_H_

#include <lv2/core/lv2.h>
#include <lv2/atom/atom.h>
#include <lv2/atom/forge.h>
#include <lv2/log/logger.h>
#include <lv2/midi/midi.h>
#include <lv2/state/state.h>
#include <lv2/options/options.h>

#include <queue>
#include <functional>

#include "../CR42YnthDSP.h"
#include "CR42YnthCommunicator.h"
#include "OSCEvent.h"

namespace cr42y
{

class CR42YnthLV2URIS;
	
class CR42YnthLV2 : public CR42YnthCommunicator
{
public:
	static CR42YnthLV2* getInstance(float samplerate = 0,
			const char* bundlePath = nullptr,
			const LV2_Feature* const * features = nullptr);
	virtual ~CR42YnthLV2();

	void activate();
	void deactivate();
	void connectPort(uint32_t port, void* data);
	void run(uint32_t n_samples);
	void cleanup();

	LV2_State_Status save(LV2_State_Store_Function store,
			LV2_State_Handle handle, uint32_t flags,
			const LV2_Feature* const * features);

	LV2_State_Status restore(LV2_State_Retrieve_Function retrieve,
			LV2_State_Handle handle, uint32_t flags,
			const LV2_Feature* const * features);

	bool isReady();

	void writeMessage(OSCEvent& event);
	
	void queueEvent(OSCEvent& event)
	{
		events_.push(new OSCEvent(event));
	}
	
	void setUIWriteFunction(std::function<void(OSCEvent&)> func)
	{
		uiWrite_ = func;
	}
	
	void log(std::string msg);

private:
	CR42YnthLV2(float samplerate, const char* bundlePath,
			const LV2_Feature* const* features);
	static CR42YnthLV2* instance;
	bool initialized;

	CR42YnthDSP* dsp;

	const LV2_Atom_Sequence* ctrlIn;
	LV2_Atom_Sequence* ctrlOut;

	float* outL;
	float* outR;

	std::vector<const float*> external;

	LV2_Log_Logger* logger;
	
	CR42YnthLV2URIS* uris_;
	LV2_Atom_Forge* forge_;
	
	LV2_Atom_Forge_Frame outFrame;
	
	std::queue<OSCEvent*> events_;
	
	std::function<void(OSCEvent&)> uiWrite_;
};

} /* namespace cr42y */

#endif /* SRC_DSP_LV2_CR42YNTHLV2_H_ */
