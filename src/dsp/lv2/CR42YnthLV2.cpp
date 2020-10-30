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
		uris(new URIS()),
		logger(new LV2_Log_Logger())
{
	const char* missing = lv2_features_query(features,
	LV2_LOG__log, &logger->log, false,
	LV2_URID__map, &map, true, nullptr);
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
	uris->msgOSCMsg = map->map(map->handle, CR42Ynth__DSP "/msg_oscmsg");
	uris->msgData = map->map(map->handle, CR42Ynth__DSP "/msg_data");
	uris->msgObj = map->map(map->handle, CR42Ynth__DSP "/msg_obj");

	forge = new LV2_Atom_Forge();
	lv2_atom_forge_init(forge, map);

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
	const char* msg = nullptr;
	int size = 0;
	void* data = nullptr;
	int dataSize = 0;
	msg = event.getMessage(&size);
	data = event.getData(&dataSize);

	//log(msg);

	lv2_atom_forge_frame_time(forge, 0);

	LV2_Atom_Forge_Frame objectFrame; // start of object with message and data property
	lv2_atom_forge_object(forge, &objectFrame, 0, uris->msgObj);

	lv2_atom_forge_key(forge, uris->msgOSCMsg); // message key
	lv2_atom_forge_vector(forge, 1, 0, size, msg); // message as char vector

	if (dataSize > 0 && data)
	{
		lv2_atom_forge_key(forge, uris->msgData); // data key
		lv2_atom_forge_vector(forge, dataSize, 0, 1, data); // data as "vector"
	}

	lv2_atom_forge_pop(forge, &objectFrame); // end object
}

void CR42YnthLV2::run(uint32_t n_samples)
{
	uint32_t capacity = ctrlOut->atom.size;
	lv2_atom_forge_set_buffer(forge, (uint8_t*) ctrlOut, 50000);
	lv2_atom_forge_sequence_head(forge, &outFrame, 0);

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
				lv2_atom_object_get(obj, uris->msgOSCMsg, &msgAtom, uris->msgData, &dataAtom, 0);
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
						dataSize = dataAtom->size - sizeof(LV2_Atom_Vector_Body);
					}

					OSCEvent oscevent(msg, msgAtom->size - sizeof(LV2_Atom_Vector_Body), data, dataSize);
					//dsp->handleEvent(&oscevent);
					handleOSCEvent(&oscevent);
				}
			}
			else if (obj->body.otype == uris->timePosition)
			{
				//log("time data");
				LV2_Atom* beat = nullptr, *bpm = nullptr, *speed = nullptr;
				lv2_atom_object_get(obj, uris->timeBarBeat, &beat, uris->timeBPM, &bpm, uris->timeSpeed, &speed, nullptr);
				if (bpm && bpm->type == uris->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/bpm", "f", ((LV2_Atom_Float*) bpm)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					//dsp->handleEvent(&event);
					handleOSCEvent(&event);
				}
				if (speed && speed->type == uris->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/speed", "f", ((LV2_Atom_Float*) speed)->body);
					OSCEvent event(buffer, len, nullptr, 0);
					//dsp->handleEvent(&event);
					handleOSCEvent(&event);
				}
				if (beat && beat->type == uris->atomFloat)
				{
					char buffer[32];
					int len = rtosc_message(buffer, 32, "/global/beat", "f", ((LV2_Atom_Float*) beat)->body);
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

void CR42YnthLV2::log(const char* msg)
{
	lv2_log_note(logger, "[CR42Ynth]: %s\n", msg);
}

} /* namespace cr42y */
