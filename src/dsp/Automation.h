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

#ifndef CR42Y_AUTOMATION_H
#define CR42Y_AUTOMATION_H

#include "OSCEventListener.h"
#include "ControlListener.h"

#include "Control.h"

namespace cr42y
{

class Automation : public ControlListener
{
public:
	Automation(uint32_t id, CR42YnthCommunicator* comm);

	virtual ~Automation();
	
	inline uint32_t id()
	{
		return id_;
	}
	
	inline uint32_t type()
	{
		return typeControl_.getValue();
	}
	
	inline float* waveform()
	{
		return waveform_;
	}
	
	inline size_t wfSize()
	{
		return wfSize_;
	}
	
	inline float deltaPhase()
	{
		return deltaPhase_;
	}
	
	inline uint32_t sustain()
	{
		return sustainControl_.getValue();
	}
	
	virtual void valueCallback(float val, Control* ctrl);
	virtual void minCallback(float min, Control* ctrl);
	virtual void maxCallback(float max, Control* ctrl);
	virtual void genCallback(std::string gen, Control* ctrl);
	
private:
	uint32_t id_;
	float* waveform_;
	size_t wfSize_;
	size_t samplerate_;
	
	float deltaPhase_;
	
	CR42YnthCommunicator* communicator_;
	
	Control typeControl_;
	Control syncControl_;
	Control useBeatsControl_;
	Control secondsControl_;
	Control beatsNumeratorControl_;
	Control beatsDenominatorControl_;
	Control sustainControl_;
	
	void setWaveform(float* waveform, size_t size);
	void updateTiming();
};

}

#endif // CR42Y_AUTOMATION_H
