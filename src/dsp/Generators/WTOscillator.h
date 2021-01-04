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


#ifndef SRC_DSP_GENERATORS_WTOSCILLATOR_H_
#define SRC_DSP_GENERATORS_WTOSCILLATOR_H_

#include "OSCEventListener.h"

namespace cr42y
{
class CR42YnthCommunicator;
class OscillatorControls;
class WavetableEditData;
class OscillatorVoiceData;

class WTOscillator : public OSCEventListener
{
public:
	WTOscillator(CR42YnthCommunicator* comm, int id, float rate);
	virtual ~WTOscillator();

	void setWavetable(std::vector<std::vector<float>>& wt);
	
	std::vector<std::vector<float>>& wavetable();

	void setEditData(WavetableEditData* ed);

	virtual void getState(std::vector<OSCEvent>& events);

	bool handleOSCEvent(OSCEvent* event);

	OscillatorControls& getControls();
	
	float getSamplerate();
	
	int getNumber();

private:
	CR42YnthCommunicator* communicator_;

	int number;

	float samplerate;

	WavetableEditData* editData;

	std::vector<std::vector<float>> wavetable_;

	OscillatorControls* controls_;
};

inline float WTOscillator::getSamplerate()
{
	return samplerate;
}

inline int WTOscillator::getNumber()
{
	return number;
}

} /* namespace cr42y */

#endif /* SRC_DSP_GENERATORS_WTOSCILLATOR_H_ */
