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

#ifndef SRC_DSP_GENERATORS_LFO_H_
#define SRC_DSP_GENERATORS_LFO_H_

#include "Generator.h"
#include "Control.h"

namespace cr42y
{

class LFO : public Generator
{
public:
	LFO(std::vector<Voice*>* vce, int id, float rate);
	virtual ~LFO();

	void nextSample();
	virtual void voiceAdded(Voice* vce);
	virtual void voiceRemoved(Voice* vce);

	virtual void sendState();

	virtual bool receiveOSCMessage(OSCEvent* event);

	float getSample(Voice* vce);

	void sync(float pos);

	void setWaveform(std::vector<float>* wf);

	void midiPanic();

private:
	int number;
	float samplerate;

	float unsyncedPhase;
	float syncedPhase;

	Control synced;
	Control retrigger;
	Control smooth;

	Control useFrequency;
	Control frequency;
	Control lenghtInNotes;

	std::vector<float>* waveform;
	std::map<Voice*, float> phases;

};

} /* namespace cr42y */

#endif /* SRC_DSP_GENERATORS_LFO_H_ */
