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


#ifndef SRC_COMMON_CONTROL_H_
#define SRC_COMMON_CONTROL_H_

#include <string>
#include <vector>

#include "OSCEventListener.h"

namespace cr42y
{
class OSCEvent;
class CR42YnthCommunicator;
class ControlListener;

class Control : public OSCEventListener
{
public:
	Control(std::string addr, CR42YnthCommunicator* comm, float val = 0, float mi = 0, float ma = 1, uint32_t gen = 0);
	virtual ~Control();

	void setValue(float val, bool callback = true, bool updateListeners = true);
	bool handleOSCEvent(OSCEvent* event);

	void getState(std::vector<OSCEvent>& events);
	void getState(std::vector<OSCEvent>& events, bool sendVal, bool sendMin, bool sendMax, bool sendGen);

	void setGenerator(uint32_t g, bool callback = true, bool updateListeners = true);

	void setMax(float m, bool callback = true, bool updateListeners = true);
	void setMin(float m, bool callback = true, bool updateListeners = true);

	std::string getAddress();
	uint32_t getGenerator();

	float getMax();
	float getMin();

	float getValue();

	void addListener(ControlListener* listener);
	void removeListener(ControlListener* listener);

protected:
	std::string address;
	CR42YnthCommunicator* communicator;
	float value;

	uint32_t generator;
	float max;
	float min;

	std::vector<ControlListener*> listeners;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_CONTROL_H_ */
