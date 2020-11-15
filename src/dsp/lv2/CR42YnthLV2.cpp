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

#include <lv2/atom/util.h>
#include <lv2/core/lv2_util.h>
#include <lv2/time/time.h>
#include <lv2/options/options.h>
#include <lv2/buf-size/buf-size.h>

#include "rtosc/rtosc.h"

#include "CR42YnthLV2.h"
#include "OSCEvent.h"
#include "common.h"

namespace cr42y
{

CR42YnthLV2* CR42YnthLV2::instance;
CR42YnthLV2* CR42YnthLV2::getInstance(float samplerate, const char* bundlePath,
		const LV2_Feature* const * features)
{
	if (!instance)
	{
		instance = new CR42YnthLV2(samplerate, bundlePath, features);
	}
	return instance;
}

CR42YnthLV2::CR42YnthLV2(float samplerate, const char* bundlePath,
		const LV2_Feature* const * features) :
		dsp(CR42YnthDSP::getInstance(samplerate, this)),
		logger(new LV2_Log_Logger()),
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
		log((char*) ("Missing feature " + std::string(missing)).c_str());
		initialized = false;
		return;
	}
	initialized = true;
	dsp->init();

	uris->atomFloat = map->map(map->handle, LV2_ATOM__Float);
	uris->atomObject = map->map(map->handle, LV2_ATOM__Object);
	uris->atomVector = map->map(map->handle, LV2_ATOM__Vector);
	uris->midiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);
	uris->timePosition = map->map(map->handle, LV2_TIME__Position);
	uris->timeBarBeat = map->map(map->handle, LV2_TIME__barBeat);
	uris->timeBPM = map->map(map->handle, LV2_TIME__beatsPerMinute);
	uris->timeSpeed = map->map(map->handle, LV2_TIME__speed);
	uris->msgOSCMsg = map->map(map->handle, CR42Ynth__OSCMSG);
	uris->msgData = map->map(map->handle, CR42Ynth__MSGDATA);
	uris->msgObj = map->map(map->handle, CR42Ynth__MSGOBJ);
	uris->msgComplete = map->map(map->handle, CR42Ynth__MSGCOMPLETE);
	uris->stateKey = map->map(map->handle, CR42Ynth__STATEKEY);
	uris->stateType = map->map(map->handle, CR42Ynth__STATETYPE);

	forge = new LV2_Atom_Forge();
	lv2_atom_forge_init(forge, map);

	ctrlIn = nullptr;
	ctrlOut = nullptr;
	outR = nullptr;
	outL = nullptr;

	ctrlOutFull_ = false;

	for (int i = 0; i < 8; i++)
	{
		external.push_back(nullptr);
	}
}

CR42YnthLV2::~CR42YnthLV2()
{
	CR42YnthDSP::destroyInstance();
	delete forge;
	forge = nullptr;
	delete uris;
	uris = nullptr;
	instance = nullptr;
}

bool CR42YnthLV2::isReady()
{
	return initialized;
}

void CR42YnthLV2::writeMessage(OSCEvent& event)
{
	if (!ctrlOutFull_)
	{
		const char* msg = nullptr;
		size_t size = 0;
		void* data = nullptr;
		size_t dataSize = 0;
		msg = event.getMessage(&size);
		data = event.getData(&dataSize);

		uint32_t space = forge->size - forge->offset;

		lv2_atom_forge_frame_time(forge, 0);

		LV2_Atom_Forge_Frame objectFrame; // start of object with message and data property
		lv2_atom_forge_object(forge, &objectFrame, 0, uris->msgObj);

		if (forge->size - forge->offset > size + 8 + 8 + sizeof(LV2_Atom_Bool) + 8)
		{
			lv2_atom_forge_key(forge, uris->msgOSCMsg); // message key
			lv2_atom_forge_vector(forge, 1, 0, size, msg); // message as char vector

			lv2_atom_forge_key(forge, uris->msgComplete);
			LV2_Atom_Forge_Ref completeRef = lv2_atom_forge_bool(forge, true);
			if (dataSize > 0 && data)
			{
				size_t available = forge->size - forge->offset;
				if (available > dataSize + 8)
				{
					lv2_atom_forge_key(forge, uris->msgData); // data key
					lv2_atom_forge_vector(forge, dataSize, 0, 1, data); // data as "vector"
				}
				else
				{
					lv2_atom_forge_key(forge, uris->msgData); // data key
					
					lv2_atom_forge_vector(forge, available, 0, 1, data); // data as "vector"
					LV2_Atom_Bool* completeAtom = (LV2_Atom_Bool*) lv2_atom_forge_deref(forge, completeRef);
					completeAtom->body = false;


					eventQueue.push(OSCEvent(msg, size, ((uint8_t*) data) + available, dataSize - available));
				}
			}
		}

		lv2_atom_forge_pop(forge, &objectFrame); // end object

		if (forge->size - forge->offset
				< sizeof(int64_t) + sizeof(LV2_Atom_Object))
		{
			ctrlOutFull_ = true;
		}
	}
	else
	{
		eventQueue.push(event);
	}
}

void CR42YnthLV2::run(uint32_t n_samples)
{
	uint32_t capacity = ctrlOut->atom.size;
	lv2_atom_forge_set_buffer(forge, (uint8_t*) ctrlOut, capacity);
	lv2_atom_forge_sequence_head(forge, &outFrame, 0);
	
	size_t queueSize = eventQueue.size();
	for (int i = 0; i < queueSize; i++)
	{
		writeMessage(eventQueue.front());
		eventQueue.pop();
	}

	for (LV2_Atom_Event* event = lv2_atom_sequence_begin(&(ctrlIn)->body);
			!lv2_atom_sequence_is_end(&(ctrlIn)->body, ctrlIn->atom.size, event);
			event = lv2_atom_sequence_next(event))
	{
		//log("event in ctrlIn");
		if (lv2_atom_forge_is_object_type(forge, event->body.type))
		{
			LV2_Atom_Object* obj = (LV2_Atom_Object*) &event->body;
			if (obj->body.otype == uris->msgObj)
			{
				//log("custom msg");
				LV2_Atom* msgAtom = nullptr;
				LV2_Atom* dataAtom = nullptr;
				lv2_atom_object_get(obj, uris->msgOSCMsg, &msgAtom,
						uris->msgData, &dataAtom, 0);
				if (msgAtom)
				{
					LV2_Atom_Vector* msgV = (LV2_Atom_Vector*) msgAtom;
					char* msg = (char*) msgV + sizeof(LV2_Atom_Vector);
					void* data = nullptr;
					int dataSize = 0;
					if (dataAtom)
					{
						LV2_Atom_Vector* dataV = (LV2_Atom_Vector*) dataAtom;
						data = (void*) ((char*) dataV + sizeof(LV2_Atom_Vector));
						dataSize = dataAtom->size
								- sizeof(LV2_Atom_Vector_Body);
					}

					OSCEvent oscevent(msg,
							msgAtom->size - sizeof(LV2_Atom_Vector_Body), data,
							dataSize);
					//dsp->handleEvent(&oscevent);
					handleOSCEvent(&oscevent);
				}
			}
			else if (obj->body.otype == uris->timePosition)
			{
				//log("time data");
				LV2_Atom* beat = nullptr, *bpm = nullptr, *speed = nullptr;
				lv2_atom_object_get(obj, uris->timeBarBeat, &beat,
						uris->timeBPM, &bpm, uris->timeSpeed, &speed, nullptr);
				if (bpm && bpm->type == uris->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/bpm", "f",
							((LV2_Atom_Float*) bpm)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					//dsp->handleEvent(&event);
					handleOSCEvent(&event);
				}
				if (speed && speed->type == uris->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/speed", "f",
							((LV2_Atom_Float*) speed)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					//dsp->handleEvent(&event);
					handleOSCEvent(&event);
				}
				if (beat && beat->type == uris->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/beat", "f",
							((LV2_Atom_Float*) beat)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					//dsp->handleEvent(&event);
					handleOSCEvent(&event);
				}
			}
		}
		if (event->body.type == uris->midiEvent)
		{
			//log("midi");
			char buffer[32];
			char* msg = (char*) (event + 1);
			char midimsg[] =
				{msg[0], msg[1], msg[2], 0};
			int len = rtosc_message(buffer, 32, "/global/midi", "m", midimsg);
			OSCEvent oscevent(buffer, len, nullptr, 0);
			//dsp->handleEvent(&oscevent);
			handleOSCEvent(&oscevent);
		}
	}

	dsp->run(n_samples);

	float* l = dsp->getOutL();
	float* r = dsp->getOutR();

	for (int i = 0; i < n_samples; i++)
	{
		outL[i] = l[i];
		outR[i] = r[i];
	}

	lv2_atom_forge_pop(forge, &outFrame);
}

void CR42YnthLV2::connectPort(uint32_t port, void* data)
{
	switch (port)
	{
	case AUDIO_OUT_L:
		outL = (float*) data;
		break;
	case AUDIO_OUT_R:
		outR = (float*) data;
		break;
	case NOTIFY:
		ctrlOut = (LV2_Atom_Sequence*) data;
		break;
	case CONTROL:
		ctrlIn = (const LV2_Atom_Sequence*) data;
		break;
	default:
		if (EXT1 <= port && port <= EXT8)
		{
			external[port - EXT1] = (const float*) data;
		}
	}
}

void CR42YnthLV2::activate()
{

}

void CR42YnthLV2::deactivate()
{

}

void CR42YnthLV2::cleanup()
{

}

LV2_State_Status CR42YnthLV2::save(LV2_State_Store_Function store,
		LV2_State_Handle handle, uint32_t flags,
		const LV2_Feature* const * features)
{
	LV2_Atom_Forge* saveForge = new LV2_Atom_Forge();
	lv2_atom_forge_init(saveForge, map);

	std::vector<OSCEvent> events;

	for (int i = 0; i < listeners_.size(); i++)
	{
		listeners_[i]->getState(events);
	}

	uint64_t bufferSize = sizeof(uint64_t) * 2;

	uint64_t msgLen = 0;
	uint64_t dataLen = 0;

	for (int i = 0; i < events.size(); i++)
	{
		events[i].getMessage(&msgLen);
		events[i].getMessage(&dataLen);

		bufferSize += sizeof(uint64_t) + msgLen + sizeof(uint64_t) + dataLen;
	}

	uint8_t buffer[bufferSize];

	uint8_t* bufferptr = buffer;

	((uint64_t*) buffer)[0] = bufferSize;
	((uint64_t*) buffer)[1] = events.size();

	bufferptr += 2 * sizeof(uint64_t);

	for (int i = 0; i < events.size(); i++)
	{
		const char* msg = events[i].getMessage(&msgLen);
		const char* data = events[i].getMessage(&dataLen);
		*(uint64_t*) bufferptr = msgLen;
		bufferptr += sizeof(uint64_t);
		memcpy(bufferptr, msg, msgLen);
		bufferptr += msgLen;
		*(uint64_t*) bufferptr = dataLen;
		bufferptr += sizeof(uint64_t);
		if (dataLen > 0 && data)
		{
			memcpy(bufferptr, data, dataLen);
			bufferptr += dataLen;
		}
	}

	return store(handle, uris->stateKey, buffer, bufferSize, uris->stateType,
			LV2_STATE_IS_PORTABLE | LV2_STATE_IS_POD);
}

LV2_State_Status CR42YnthLV2::restore(LV2_State_Retrieve_Function retrieve,
		LV2_State_Handle handle, uint32_t flags,
		const LV2_Feature* const * features)
{
	size_t size = 0;
	uint32_t type = 0;
	uint32_t dataFlags = 0;
	const void* data = retrieve(handle, uris->stateKey, &size, &type,
			&dataFlags);

	if (!data)
	{
		return LV2_STATE_ERR_UNKNOWN;
	}

	uint8_t* bufferptr = (uint8_t*) data;
	uint64_t bufferSize = ((uint64_t*) bufferptr)[0];
	uint64_t eventAmount = ((uint64_t*) bufferptr)[1];

	bufferptr += 2 * sizeof(uint64_t);

	for (int i = 0; i < eventAmount; i++)
	{
		uint64_t msgLen = *((uint64_t*) bufferptr);
		bufferptr += sizeof(uint64_t);

		char msg[msgLen];
		memcpy(msg, bufferptr, msgLen);
		bufferptr += msgLen;

		uint64_t dataLen = *((uint64_t*) bufferptr);
		bufferptr += sizeof(uint64_t);

		char* data = nullptr;
		if (dataLen > 0)
		{
			data = new char[dataLen];
			memcpy(data, bufferptr, dataLen);
		}
		OSCEvent event(msg, msgLen, data, dataLen);
		handleOSCEvent(&event);
		if (data)
		{
			delete data;
		}
	}

	return LV2_STATE_SUCCESS;
}

void CR42YnthLV2::log(const char* msg)
{
	lv2_log_note(logger, "[CR42Ynth]: %s\n", msg);
}

} /* namespace cr42y */
