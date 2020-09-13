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

#include "../CR42YnthDSP.h"
#include "CR42YnthCommunicator.h"

namespace cr42y
{

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

	bool isReady();

	void writeMessage(char* msg, int size, void* data, int dataSize);

	void log(const char* msg);

private:
	CR42YnthLV2(float samplerate, const char* bundlePath,
			const LV2_Feature* const * features);
	static CR42YnthLV2* instance;
	bool initialized;

	CR42YnthDSP* dsp;

	const LV2_Atom_Sequence* ctrlIn;
	LV2_Atom_Sequence* ctrlOut;

	float* outL;
	float* outR;

	std::vector<const float*> external;

	LV2_Atom_Forge* forge;
	LV2_URID_Map* map;
	LV2_Log_Logger* logger;

	LV2_Atom_Forge_Frame outFrame;

	class URIS
	{
	public:
		LV2_URID atomFloat;
		LV2_URID atomObject;
		LV2_URID atomVector;
		LV2_URID midiEvent;
		LV2_URID timePosition;
		LV2_URID timeBarBeat;
		LV2_URID timeBPM;
		LV2_URID timeSpeed;

		LV2_URID msgOSCMsg;
		LV2_URID msgData;
		LV2_URID msgObj;
	};
	URIS* uris;
};

} /* namespace cr42y */

#endif /* SRC_DSP_LV2_CR42YNTHLV2_H_ */
