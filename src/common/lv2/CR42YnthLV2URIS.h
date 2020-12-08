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

#ifndef CR42Y_CR42YNTHLV2URIS_H
#define CR42Y_CR42YNTHLV2URIS_H

#include <lv2/urid/urid.h>

namespace cr42y {


class CR42YnthLV2URIS
{
public:
    CR42YnthLV2URIS(LV2_URID_Map* map);
    virtual ~CR42YnthLV2URIS();
	
	
	LV2_URID atomFloat;
	LV2_URID atomObject;
	LV2_URID atomVector;
	LV2_URID atomEventTransfer;
	
	LV2_URID bufsizeSequenceSize;
	
	LV2_URID midiEvent;
	LV2_URID timePosition;
	LV2_URID timeBarBeat;
	LV2_URID timeBPM;
	LV2_URID timeSpeed;
	
	LV2_URID msgOSCMsg;
	LV2_URID msgData;
	LV2_URID msgObj;
	LV2_URID msgComplete;
	
	LV2_URID stateKey;
	LV2_URID stateType;
	
	LV2_URID_Map* map();

private:
	LV2_URID_Map* map_;
};

}

#endif // CR42Y_CR42YNTHLV2URIS_H
