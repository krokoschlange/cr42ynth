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

#include <lv2/core/lv2_util.h>
#include <lv2/time/time.h>
#include <lv2/midi/midi.h>
#include <lv2/buf-size/buf-size.h>

#include "rtosc/rtosc.h"

#include "CR42YnthUILV2.h"
#include "common.h"
#include "OSCEvent.h"

namespace cr42y
{

CR42YnthUI_LV2::CR42YnthUI_LV2(const char* bundlePath,
		LV2UI_Write_Function writeFunction, LV2UI_Controller ctrler,
		LV2UI_Widget* widget, const LV2_Feature* const * features) :
		CR42YnthCommunicator(),
		logger(new LV2_Log_Logger()),
		write(writeFunction),
		controller(ctrler),
		forge(new LV2_Atom_Forge()),
		uris(new URIS())

{
	const char* missing = lv2_features_query(features,
	LV2_LOG__log, &logger->log, false,
	LV2_URID__map, &map, true,
	LV2_OPTIONS__options, lv2Options_, true,
	nullptr);
	if (map)
	{
		lv2_log_logger_set_map(logger, map);
	}
	if (missing)
	{
		log(("UI Missing feature " + std::string(missing)).c_str());
		ready = false;
		return;
	}
	ready = true;
	lv2_atom_forge_init(forge, map);

	uris->atomFloat = map->map(map->handle, LV2_ATOM__Float);
	uris->atomObject = map->map(map->handle, LV2_ATOM__Object);
	uris->atomVector = map->map(map->handle, LV2_ATOM__Vector);
	uris->atomEventTransfer = map->map(map->handle, LV2_ATOM__eventTransfer);
	
	uris->bufsizeSequenceSize = map->map(map->handle, LV2_BUF_SIZE__sequenceSize);

	uris->midiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);

	uris->timePosition = map->map(map->handle, LV2_TIME__Position);
	uris->timeBarBeat = map->map(map->handle, LV2_TIME__barBeat);
	uris->timeBPM = map->map(map->handle, LV2_TIME__beatsPerMinute);
	uris->timeSpeed = map->map(map->handle, LV2_TIME__speed);

	uris->msgOSCMsg = map->map(map->handle, CR42Ynth__OSCMSG);
	uris->msgData = map->map(map->handle, CR42Ynth__MSGDATA);
	uris->msgObj = map->map(map->handle, CR42Ynth__MSGOBJ);

	LV2UI_Resize* resize = nullptr;
	intptr_t parent = 0;
	for (int i = 0; features[i]; i++)
	{
		if (!strcmp(features[i]->URI, LV2_UI__parent))
		{
			parent = (intptr_t) features[i]->data;
		}
		else if (!strcmp(features[i]->URI, LV2_UI__resize))
		{
			resize = (LV2UI_Resize*) features[i]->data;
		}
	}

	ui = new CR42YnthUI(this, bundlePath);
	*widget = (void*) ui->gobj();
	if (resize)
	{
		resize->ui_resize(resize->handle, ui->get_width(), ui->get_height());
	}
	
	for (const LV2_Options_Option* op = lv2Options_; op; op++)
	{
		scanOption(op);
	}
}

CR42YnthUI_LV2::~CR42YnthUI_LV2()
{
	delete forge;
	//delete ui; //TODO: fix this
}

bool CR42YnthUI_LV2::isReady()
{
	return ready;
}

void CR42YnthUI_LV2::writeMessage(OSCEvent& event)
{
	const char* msg = nullptr;
	size_t size = 0;
	void* data = nullptr;
	size_t dataSize = 0;
	msg = event.getMessage(&size);
	data = event.getData(&dataSize);


	size_t bufferSize = size + dataSize + sizeof(LV2_Atom_Object)
			+ 2 * sizeof(LV2_Atom_Vector) + 256;
	uint8_t buffer[bufferSize];
	lv2_atom_forge_set_buffer(forge, buffer, bufferSize);

	LV2_Atom_Forge_Frame frame;
	LV2_Atom* msgAtom = (LV2_Atom*) lv2_atom_forge_object(forge, &frame, 0,
			uris->msgObj);
	lv2_atom_forge_key(forge, uris->msgOSCMsg);
	lv2_atom_forge_vector(forge, 1, 0, size, msg);
	if (data && dataSize > 0)
	{
		lv2_atom_forge_key(forge, uris->msgData);
		lv2_atom_forge_vector(forge, dataSize, 0, 1, data);
	}
	lv2_atom_forge_pop(forge, &frame);

	write(controller, CONTROL, lv2_atom_total_size(msgAtom),
			uris->atomEventTransfer, msgAtom);
}

void CR42YnthUI_LV2::log(const char* msg)
{
	lv2_log_note(logger, "[CR42Ynth UI]: %s\n", msg);
}

void CR42YnthUI_LV2::portEvent(uint32_t port, uint32_t bufferSize,
		uint32_t format, const void* buffer)
{
	switch (port)
	{
	case NOTIFY:
		if (format == uris->atomEventTransfer)
		{
			const LV2_Atom* atom = (const LV2_Atom*) buffer;
			if (lv2_atom_forge_is_object_type(forge, atom->type))
			{
				const LV2_Atom_Object* obj = (const LV2_Atom_Object*) atom;
				if (obj->body.otype == uris->msgObj)
				{
					LV2_Atom* msgAtom = nullptr;
					LV2_Atom* dataAtom = nullptr;
					lv2_atom_object_get(obj, uris->msgOSCMsg, &msgAtom,
							uris->msgData, &dataAtom, 0);
					if (msgAtom)
					{
						LV2_Atom_Vector* msgVector = (LV2_Atom_Vector*) msgAtom;
						char* msg = (char*) msgVector + sizeof(LV2_Atom_Vector);
						void* data = nullptr;
						int dataSize = 0;
						if (dataAtom)
						{
							data = (void*) ((char*) dataAtom
									+ sizeof(LV2_Atom_Vector));
							dataSize = dataAtom->size - sizeof(LV2_Atom_Vector_Body);
						}
						OSCEvent event(msg, msgAtom->size - sizeof(LV2_Atom_Vector_Body), data, dataSize);

						//log(msg);

						bool handled = handleOSCEvent(&event);
						/*if (handled)
						{
							log("handled");
						}
						else
						{
							log("failed");
						}*/
					}
				}
			}
		}
	}
}

int CR42YnthUI_LV2::idle()
{
	ui->idle();
	return 0;
}

void CR42YnthUI_LV2::scanOption(const LV2_Options_Option* option)
{
	if (option->key == uris->bufsizeSequenceSize)
	{
		uint8_t bytes[64];
		memcpy(bytes + (64 - option->size), option->value, option->size);
		portMaxSize_ = *((uint64_t*) bytes);
	}
}

} /* namespace cr42y */
