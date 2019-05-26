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
#include <lv2/atom/forge.h>

#include "PortCommunicator.h"
#include "../../DefinitionHandler.h"

namespace cr42y
{

PortCommunicator::PortCommunicator(LV2_URID_Map* m, LV2_Atom_Sequence* in,
		LV2_Atom_Sequence* out) :
				map(m),
				inPort(in),
				outPort(out)
{
}

PortCommunicator::~PortCommunicator()
{
}

void PortCommunicator::receiveEvents()
{
	LV2_Atom_Event* event = lv2_atom_sequence_begin(&inPort->body);
	while(!lv2_atom_sequence_is_end(&inPort->body, inPort->atom.size, event))
	{
		LV2_Atom_Forge* forge;
		lv2_atom_forge_init(forge, map);
		if (lv2_atom_forge_is_object_type(forge, event->body.type))
		{
			LV2_Atom_Object* object = (LV2_Atom_Object*) &event->body;

			for (MessageReceiver* rec : receivers)
			{
				LV2_Atom_Int* messageType;
				lv2_atom_object_get_typed(object, DefinitionHandler::getInstance()->msg_type, &messageType, DefinitionHandler::getInstance()->atom_int);
				if (rec->getMessageType() == messageType->body)
				{
					rec->receive(object);
				}
			}
		}
		event = lv2_atom_sequence_next(event);
	}
}

void PortCommunicator::addReceiver(MessageReceiver* rec)
{
	for (MessageReceiver* r : receivers)
	{
		if (r == rec)
		{
			return;
		}
	}
	receivers.push_back(rec);
}

void PortCommunicator::removeReceiver(MessageReceiver* rec)
{
	for (int i = 0; i < receivers.size(); i++)
	{
		if (receivers[i] == rec)
		{
			receivers.erase(receivers.begin() + i);
			i--;
		}
	}
}

} /* namespace cr42y */
