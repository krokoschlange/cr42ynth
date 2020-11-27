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
#include <lv2/atom/util.h>
#include <utility>

#include "rtosc/rtosc.h"

#include "CR42YnthUILV2.h"
#include "common.h"
#include "lv2_common.h"
#include "OSCEvent.h"

namespace cr42y
{

CR42YnthUI_LV2::CR42YnthUI_LV2(const char* bundlePath,
		LV2UI_Write_Function writeFunction, LV2UI_Controller ctrler,
		LV2UI_Widget* widget, const LV2_Feature* const * features) :
		CR42YnthLV2Communicator(),
		logger(new LV2_Log_Logger()),
		write(writeFunction),
		controller(ctrler),
		forge(new LV2_Atom_Forge()),
		uris(new URIS()),
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
	uris->msgComplete = map->map(map->handle, CR42Ynth__MSGCOMPLETE);

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
	/*const char* msg = nullptr;
	size_t size = 0;
	void* data = nullptr;
	size_t dataSize = 0;
	msg = event.getMessage(&size);
	data = event.getData(&dataSize);


	size_t bufferSize = size + dataSize + sizeof(LV2_Atom_Object)
			+ 2 * sizeof(LV2_Atom_Vector) + 256;
	uint8_t buffer[bufferSize];
	lv2_atom_forge_set_buffer(forge, buffer, bufferSize);


	size_t maxAvailableAtomSize = portAvailableSpace_ - sizeof(LV2_Atom_Event::time);

	size_t propertySize = sizeof(LV2_Atom_Property_Body) - sizeof(LV2_Atom);

	if (maxAvailableAtomSize > size + sizeof(LV2_Atom_Object) + propertySize +
			sizeof(LV2_Atom_Vector) + propertySize + sizeof(LV2_Atom_Int))
	{
		LV2_Atom_Forge_Frame frame;
		LV2_Atom* msgAtom = (LV2_Atom*) lv2_atom_forge_object(forge, &frame, 0,
				uris->msgObj);

		lv2_atom_forge_key(forge, uris->msgComplete);
		LV2_Atom_Forge_Ref completeRef = lv2_atom_forge_int(forge, flags);

		lv2_atom_forge_key(forge, uris->msgOSCMsg);
		lv2_atom_forge_vector(forge, 1, 0, size, msg);

		maxAvailableAtomSize -= forge->offset;
		if (data && dataSize > 0)
		{
			if (maxAvailableAtomSize > dataSize + sizeof(LV2_Atom_Property_Body)
					- sizeof(LV2_Atom) + sizeof(LV2_Atom_Vector))
			{
				lv2_atom_forge_key(forge, uris->msgData);
				lv2_atom_forge_vector(forge, dataSize, 0, 1, data);

				LV2_Atom_Int* completeAtom = (LV2_Atom_Int*) lv2_atom_forge_deref(forge, completeRef);
				completeAtom->body |= HAS_DATA_END;
			}
			else
			{

				lv2_atom_forge_key(forge, uris->msgData);
				lv2_atom_forge_vector(forge, maxAvailableAtomSize, 0, 1, data);
				
				eventQueue_.push(std::pair<OSCEvent, int>(
							OSCEvent(msg, size, ((uint8_t*)data)
								+ maxAvailableAtomSize, maxAvailableAtomSize), 0));

				LV2_Atom_Int* completeAtom = (LV2_Atom_Int*) lv2_atom_forge_deref(forge, completeRef);
				completeAtom->body &= ~HAS_DATA_END;

			}
		}
		lv2_atom_forge_pop(forge, &frame);

		write(controller, CONTROL, lv2_atom_total_size(msgAtom),
				uris->atomEventTransfer, msgAtom);

		portAvailableSpace_ -= lv2_atom_pad_size(lv2_atom_total_size(msgAtom));
		portAvailableSpace_ -= sizeof(LV2_Atom_Event::time);
	}
	else
	{
		eventQueue_.push(std::pair<OSCEvent, int>(event, flags));
	}*/
	const char* msg = nullptr;
	size_t msgSize = 0;
	uint8_t* data = nullptr;
	size_t dataSize = 0;

	msg = event.getMessage(&msgSize);
	data = event.getData(&dataSize);

	queueMsg(msg, msgSize, data, dataSize);
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

bool CR42YnthUI_LV2::handleOSCEvent(OSCEvent* event)
{
	std::string pattern = "/global/port_notify";
	char* end = nullptr;
	rtosc_match_path(pattern.c_str(), event->getMessage(), (const char**) &end);
	if (end && *end == '\0')
	{
		portAvailableSpace_ = portMaxSize_;

		size_t queueSize = eventQueue_.size();
		for (int i = 0; i < queueSize; ++i) {
			writeMessageWithFlags(eventQueue_.front().first, eventQueue_.front().second);
			eventQueue_.pop();
		}
		return true;
	}

	return CR42YnthCommunicator::handleOSCEvent(event);
}



} /* namespace cr42y */
