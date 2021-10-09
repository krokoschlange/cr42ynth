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
#include "lv2_common.h"

#include "CR42YnthLV2URIS.h"


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

CR42YnthLV2::CR42YnthLV2(float samplerate, const char*,
		const LV2_Feature* const * features) :
		dsp(CR42YnthDSP::getInstance(samplerate, this)),
		logger(new LV2_Log_Logger()),
		uris_(nullptr),
		forge_(new LV2_Atom_Forge())
{
	LV2_URID_Map* map = nullptr;
	
	const char* missing = lv2_features_query(features,
	LV2_LOG__log, &logger->log, false,
	LV2_URID__map, &map, true,
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

	uris_ = new CR42YnthLV2URIS(map);


	ctrlIn = nullptr;
	ctrlOut = nullptr;
	outR = nullptr;
	outL = nullptr;

	for (int i = 0; i < 8; i++)
	{
		external.push_back(nullptr);
	}
}

CR42YnthLV2::~CR42YnthLV2()
{
	CR42YnthDSP::destroyInstance();
	instance = nullptr;
	
	delete uris_;
	delete logger;
}

bool CR42YnthLV2::isReady()
{
	return initialized;
}

void CR42YnthLV2::writeMessage(OSCEvent& event)
{
	uiWrite_(event);
}

void CR42YnthLV2::run(uint32_t n_samples)
{
	//uint32_t capacity = ctrlOut->atom.size;
	//lv2_atom_forge_set_buffer(forge_, (uint8_t*) ctrlOut, capacity);
	//lv2_atom_forge_sequence_head(forge_, &outFrame, 0);
	
	while (events_.size() > 0)
	{
		handleOSCEvent(events_.front());
		delete events_.front();
		events_.pop();
	}

	for (LV2_Atom_Event* event = lv2_atom_sequence_begin(&(ctrlIn)->body);
			!lv2_atom_sequence_is_end(&(ctrlIn)->body, ctrlIn->atom.size, event);
			event = lv2_atom_sequence_next(event))
	{
		if (lv2_atom_forge_is_object_type(forge_, event->body.type))
		{
			LV2_Atom_Object* obj = (LV2_Atom_Object*) &event->body;
			if (obj->body.otype == uris_->timePosition)
			{
				LV2_Atom* beat = nullptr, *bpm = nullptr, *speed = nullptr;
				lv2_atom_object_get(obj, uris_->timeBeat, &beat,
									uris_->timeBPM, &bpm, uris_->timeSpeed, &speed, nullptr);
				if (bpm && bpm->type == uris_->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/bpm", "sf",
							"set_value", ((LV2_Atom_Float*) bpm)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					handleOSCEvent(&event);
				}
				if (speed && speed->type == uris_->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/speed", "sf",
							"set_value", ((LV2_Atom_Float*) speed)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					handleOSCEvent(&event);
				}
				if (beat && beat->type == uris_->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/beat", "sf",
							"set_value", ((LV2_Atom_Float*) beat)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					handleOSCEvent(&event);
				}
			}
		}
		if (event->body.type == uris_->midiEvent)
		{
			char buffer[32];
			char* msg = (char*) (event + 1);
			char midimsg[] =
				{msg[0], msg[1], msg[2], 0};
			int len = rtosc_message(buffer, 32, "/global/midi", "m", midimsg);
			OSCEvent oscevent(buffer, len, nullptr, 0);
			handleOSCEvent(&oscevent);
		}
	}

	dsp->run(n_samples);

	/*float* l = dsp->getOutL();
	float* r = dsp->getOutR();

	for (int i = 0; i < n_samples; i++)
	{
		outL[i] = l[i];
		outR[i] = r[i];
	}*/

	/*char buffer[32];
	size_t msgSize = rtosc_message(buffer, 32, "/global/port_notify", "");
	
	writeSingleMessage(createCommunicatorMessage(buffer, msgSize, nullptr, 0));
	
	writeQueue();
	
	lv2_atom_forge_pop(forge_, &outFrame);*/
}

void CR42YnthLV2::connectPort(uint32_t port, void* data)
{
	switch (port)
	{
	case AUDIO_OUT_L:
		outL = (float*) data;
		dsp->setSink(outL, outR);
		break;
	case AUDIO_OUT_R:
		outR = (float*) data;
		dsp->setSink(outL, outR);
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
		LV2_State_Handle handle, uint32_t,
		const LV2_Feature* const *)
{
	LV2_Atom_Forge* saveForge = new LV2_Atom_Forge();
	lv2_atom_forge_init(saveForge, uris_->map());

	std::vector<OSCEvent> events;

	for (size_t i = 0; i < listeners_.size(); i++)
	{
		if (listeners_[i]->hasPriority())
		{
			listeners_[i]->getState(events);
		}
	}
	for (size_t i = 0; i < listeners_.size(); i++)
	{
		if (!listeners_[i]->hasPriority())
		{
			listeners_[i]->getState(events);
		}
	}

	uint64_t bufferSize = sizeof(uint64_t) * 2;

	uint64_t msgLen = 0;
	uint64_t dataLen = 0;

	for (size_t i = 0; i < events.size(); i++)
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

	for (size_t i = 0; i < events.size(); i++)
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

	return store(handle, uris_->stateKey, buffer, bufferSize, uris_->stateType,
			LV2_STATE_IS_PORTABLE | LV2_STATE_IS_POD);
}

LV2_State_Status CR42YnthLV2::restore(LV2_State_Retrieve_Function retrieve,
		LV2_State_Handle handle, uint32_t,
		const LV2_Feature* const *)
{
	size_t size = 0;
	uint32_t type = 0;
	uint32_t dataFlags = 0;
	const void* data = retrieve(handle, uris_->stateKey, &size, &type,
			&dataFlags);

	if (!data)
	{
		return LV2_STATE_ERR_UNKNOWN;
	}

	uint8_t* bufferptr = (uint8_t*) data;
	//uint64_t bufferSize = ((uint64_t*) bufferptr)[0];
	uint64_t eventAmount = ((uint64_t*) bufferptr)[1];

	bufferptr += 2 * sizeof(uint64_t);

	for (uint64_t i = 0; i < eventAmount; i++)
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

void CR42YnthLV2::log(std::string msg)
{
	lv2_log_note(logger, "[CR42Ynth]: %s\n", msg.c_str());
}

} /* namespace cr42y */
