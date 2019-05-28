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

#include "DefinitionHandler.h"

#include <lv2/atom/atom.h>


namespace cr42y
{
DefinitionHandler* DefinitionHandler::instance;

DefinitionHandler::DefinitionHandler()
{
}

DefinitionHandler::~DefinitionHandler()
{
}

DefinitionHandler* DefinitionHandler::getInstance()
{
	if (!instance)
	{
		instance = new DefinitionHandler();
	}
	return instance;
}

void DefinitionHandler::load(LV2_URID_Map* m)
{
	thisMap = m;
	atom_obj = map(LV2_ATOM__Object);
	atom_int = map(LV2_ATOM__Int);
	atom_double = map(LV2_ATOM__Double);
	atom_bool = map(LV2_ATOM__Bool);

	cr42ynth_uri = map(CR42YnthURI);
	cr42ynth_ui_uri = map(CR42YnthUIURI);
	msg_type = map(MSG_TYPE);
	msg_key = map(MSG_KEY);
}

LV2_URID DefinitionHandler::map(std::string uri)
{
	return thisMap->map(thisMap->handle, uri.c_str());
}
} /* namespace cr42y */
