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

#include "CR42YnthLV2URIS.h"

#include <lv2/atom/atom.h>
#include <lv2/buf-size/buf-size.h>
#include <lv2/midi/midi.h>
#include <lv2/time/time.h>

#include "lv2_common.h"

namespace cr42y
{

CR42YnthLV2URIS::CR42YnthLV2URIS(LV2_URID_Map* map) :
		map_(map)
{
	atomFloat = map->map(map->handle, LV2_ATOM__Float);
	atomObject = map->map(map->handle, LV2_ATOM__Object);
	atomVector = map->map(map->handle, LV2_ATOM__Vector);
	atomEventTransfer = map->map(map->handle, LV2_ATOM__eventTransfer);
	
	bufsizeSequenceSize = map->map(map->handle,
						  LV2_BUF_SIZE__sequenceSize);
	
	midiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);
	
	timePosition = map->map(map->handle, LV2_TIME__Position);
	timeBarBeat = map->map(map->handle, LV2_TIME__barBeat);
	timeBPM = map->map(map->handle, LV2_TIME__beatsPerMinute);
	timeSpeed = map->map(map->handle, LV2_TIME__speed);
	
	msgOSCMsg = map->map(map->handle, CR42Ynth__OSCMSG);
	msgData = map->map(map->handle, CR42Ynth__MSGDATA);
	msgObj = map->map(map->handle, CR42Ynth__MSGOBJ);
	msgComplete = map->map(map->handle, CR42Ynth__MSGCOMPLETE);
	
	stateKey = map->map(map->handle, CR42Ynth__STATEKEY);
	stateType = map->map(map->handle, CR42Ynth__STATETYPE);
}

CR42YnthLV2URIS::~CR42YnthLV2URIS()
{

}

LV2_URID_Map* CR42YnthLV2URIS::map()
{
	return map_;
}

}
