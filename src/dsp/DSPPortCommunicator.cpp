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

#include "DSPPortCommunicator.h"
#include "../common/DefinitionHandler.h"

namespace cr42y
{

DSPPortCommunicator::DSPPortCommunicator(LV2_Atom_Sequence* i,
		LV2_Atom_Sequence* o, LV2_URID_Map* m) :
		PortCommunicator(m),
		in(i),
		out(o)
{
	lv2_atom_forge_init(&forge, map);
}

DSPPortCommunicator::~DSPPortCommunicator()
{
}

void DSPPortCommunicator::receiveEvents()
{
	LV2_Atom_Forge forge;
	lv2_atom_forge_init(&forge, map);
	for (LV2_Atom_Event* event = lv2_atom_sequence_begin(&in->body);
			!lv2_atom_sequence_is_end(&in->body, in->atom.size, event);
			lv2_atom_sequence_next(event))
	{
		if (lv2_atom_forge_is_object_type(&forge, event->body.type))
		{
			LV2_Atom_Object* object = (LV2_Atom_Object*) &event->body;
			LV2_Atom_Int* msgType;
			lv2_atom_object_get_typed(object,
					DefinitionHandler::getInstance()->msg_type, &msgType,
					DefinitionHandler::getInstance()->atom_int);
			for (int i = 0; i < receivers.size(); i++)
			{
				if (receivers[i]->getMessageType() == msgType->body)
				{
					LV2_Atom_Object* data;
					lv2_atom_object_get_typed(object,
							DefinitionHandler::getInstance()->msg_object, &data,
							DefinitionHandler::getInstance()->atom_obj);
					if (data)
					{
						if (receivers[i]->getCheckerFunction())
						{
							if (receivers[i]->getCheckerFunction()(data))
							{
								receivers[i]->receive(data);
							}
						}
						else
						{
							receivers[i]->receive(data);
						}
					}
				}
			}
		}
	}
}

void DSPPortCommunicator::sendEvent(int msgType, LV2_Atom* data)
{
	LV2_Atom_Forge_Frame* frame;
	const uint32_t capacity = out->atom.size;
	lv2_atom_forge_set_buffer(&forge, (uint8_t*) out, capacity);
	lv2_atom_forge_sequence_head(&forge, frame, 0);
	lv2_atom_forge_frame_time(&forge, 0);
	
	//Atom
	LV2_Atom_Forge_Frame* objectFrame;
	LV2_Atom* object = (LV2_Atom*) lv2_atom_forge_object(&forge, objectFrame, 0,
			DefinitionHandler::getInstance()->msg_object_type);
	
	//msgType
	lv2_atom_forge_key(&forge, DefinitionHandler::getInstance()->msg_type);
	lv2_atom_forge_int(&forge, msgType);
	
	//Atom Object
	lv2_atom_forge_key(&forge, DefinitionHandler::getInstance()->msg_object);
	lv2_atom_forge_primitive(&forge, data);
	lv2_atom_forge_pop(&forge, objectFrame);
	
	lv2_atom_forge_pop(&forge, frame);
}

} /* namespace cr42y */
