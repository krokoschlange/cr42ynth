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

#ifndef SRC_DSP_CR42YNTH_H_
#define SRC_DSP_CR42YNTH_H_

#include "lv2/core/lv2.h"
#include "lv2/urid/urid.h"

#include "WTOscillator.h"
#include "Envelope.h"
#include "LFO.h"
#include "Voice.h"
#include "CR42YnthURIS.h"

#include <vector>

namespace cr42y
{

enum {
	CR42YNTH_MIDI = 0,
	CR42YNTH_OUT0,
	CR42YNTH_OUT1,
	CR42YNTH_CTRL,
	CR42YNTH_NOTIFY
};

class CR42Ynth
{
public:
	virtual ~CR42Ynth();

	static CR42Ynth* getInstance(double sampleRate, const char* bundlePath,
			const LV2_Feature* const * features);
	static CR42Ynth* getInstance();

	static LV2_Handle lv2Instantiate(const LV2_Descriptor* descriptor,
			double sampleRate, const char* bundlePath,
			const LV2_Feature* const * features);
	static void lv2ConnectPort(LV2_Handle instance, uint32_t port, void* data);
	static void lv2Activate(LV2_Handle instance);
	static void lv2Run(LV2_Handle instance, uint32_t n_samples);
	static void lv2Deactivate(LV2_Handle instance);
	static void lv2Cleanup(LV2_Handle instance);
	static const void* lv2ExtensionData(const char* uri);

	void connectPort(uint32_t port, void* data);
	void activate();
	void run(uint32_t n_samples);
	void deactivate();

	void newVoice(int note);

	void controlSubscribe(Control* ctrl);
	void controlUnsubscribe(Control* ctrl);

	LV2_URID_Map* map;
	CR42YnthURIS* urids;

private:
	CR42Ynth(double rate, const char* bundlePath,
			const LV2_Feature* const * features);
	static CR42Ynth* singleton;

	WTOscillator* oscillators;
	std::vector<LFO*> lfos;
	std::vector<Envelope*> envelopes;
	std::vector<Voice*> voices;

	std::vector<Control*> controls;
};

} /* namespace cr42y */

#endif /* SRC_DSP_CR42YNTH_H_ */
