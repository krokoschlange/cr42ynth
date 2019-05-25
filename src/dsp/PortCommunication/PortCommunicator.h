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

#ifndef SRC_DSP_PORTCOMMUNICATION_PORTCOMMUNICATOR_H_
#define SRC_DSP_PORTCOMMUNICATION_PORTCOMMUNICATOR_H_

#include <vector>
#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>

#include "MessageReceiver.h"

namespace cr42y
{

class PortCommunicator
{
public:
	PortCommunicator(LV2_URID_Map* m, LV2_Atom_Sequence* in, LV2_Atom_Sequence* out);
	virtual ~PortCommunicator();

	void receiveEvents();
	void sendEvent(int messageType, LV2_Atom* data);

	void addReceiver(MessageReceiver* rec);
	void removeReceiver(MessageReceiver* rec);

private:
	LV2_URID_Map* map;
	LV2_Atom_Sequence* inPort;
	LV2_Atom_Sequence* outPort;
	std::vector<MessageReceiver*> receivers;
};

} /* namespace cr42y */

#endif /* SRC_DSP_PORTCOMMUNICATION_PORTCOMMUNICATOR_H_ */
