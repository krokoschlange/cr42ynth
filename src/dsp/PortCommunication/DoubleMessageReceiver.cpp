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

#include <lv2/atom/util.h>

#include "DoubleMessageReceiver.h"
#include "../../DefinitionHandler.h"


namespace cr42y
{

DoubleMessageReceiver::DoubleMessageReceiver(PortCommunicator comm, int type,
		std::function<void(double)> setter, LV2_URID* dKey = nullptr) :
				messageType(type),
				setterFunction(setter),
				dataKey(dKey)
{
	comm.addReceiver(this);
}

DoubleMessageReceiver::~DoubleMessageReceiver()
{
}

int DoubleMessageReceiver::getMessageType()
{
	return messageType;
}

void DoubleMessageReceiver::receive(LV2_Atom_Object* obj)
{
	LV2_Atom_Double* value;
	LV2_URID* keyURID = dataKey ? dataKey : DefinitionHandler::msg_key;
	lv2_atom_object_get_typed(obj, keyURID, &value, DefinitionHandler::atom_double);
	setterFunction(value->body);
}

} /* namespace cr42y */
