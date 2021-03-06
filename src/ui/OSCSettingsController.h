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
#ifndef SRC_UI_OSCSETTINGSCONTROLLER_H_
#define SRC_UI_OSCSETTINGSCONTROLLER_H_

#include "OSCEventListener.h"

#include <vector>
#include <deque>

namespace cr42y
{
class OscillatorControls;
class WavetableEditData;
class CR42YnthCommunicator;
class WavetableEditController;

class OSCSettingsController : OSCEventListener
{
public:
	OSCSettingsController(CR42YnthCommunicator* communicator, WavetableEditController* wtEditController);
	virtual ~OSCSettingsController();

	WavetableEditData* getEditData(int idx);
	OscillatorControls* getControls(int idx);

	bool handleOSCEvent(OSCEvent* event);
	void getState(std::vector<OSCEvent>& events);

	void wavetableChangedCallback();

	void eraseHistoryCallback(std::deque<std::pair<char*, std::vector<int>>>);

private:
	CR42YnthCommunicator* communicator_;
	WavetableEditController* wtEditController_;

	std::vector<WavetableEditData> editData_;
	std::vector<OscillatorControls*> controls_;

	std::vector<std::deque<std::pair<char*, std::vector<int>>>> editHistory_;
};

} /* namespace cr42y */

#endif /* SRC_UI_OSCSETTINGSCONTROLLER_H_ */
