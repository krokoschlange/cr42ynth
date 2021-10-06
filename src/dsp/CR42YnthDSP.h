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
#ifndef SRC_DSP_CR42YNTHDSP_H_
#define SRC_DSP_CR42YNTHDSP_H_

#include <vector>
#include <string>

#include "OSCEventListener.h"

namespace cr42y
{
class WTOscillator;
class Generator;
class Voice;
class CR42YnthCommunicator;
class OSCEvent;
class Control;
class ModulationControls;
class AutomationHandler;

class CR42YnthDSP : OSCEventListener
{
public:
	static CR42YnthDSP* getInstance(float rate = 0, CR42YnthCommunicator* comm = nullptr);
	static void destroyInstance();

	void init();

	float getBPM();
	float getSamplerate();
	CR42YnthCommunicator* getCommunicator();

	std::vector<WTOscillator*> getOscillators();

	void run(uint32_t n_samples);
	bool handleOSCEvent(OSCEvent* event);
	
	void calculateOscillators(float* left, float* right, uint32_t samples);

	void getState(std::vector<OSCEvent>& events);

	void setSink(float* left, float* right);
	float* getOutR();
	float* getOutL();

private:
	static CR42YnthDSP* instance;
	CR42YnthDSP(float rate, CR42YnthCommunicator* comm);
	virtual ~CR42YnthDSP();


	float samplerate;
	CR42YnthCommunicator* communicator;

	Control* bpm;
	Control* vol;

	std::vector<WTOscillator*> oscillators;
		
	ModulationControls* modCtrls_;
	AutomationHandler* automationHandler_;

	std::vector<Voice*> voices;

	float* outR;
	float* outL;
};

} /* namespace cr42y */

#endif /* SRC_DSP_CR42YNTHDSP_H_ */
