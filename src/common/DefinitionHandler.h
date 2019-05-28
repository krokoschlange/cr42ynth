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

#ifndef SRC_COMMON_DEFINITIONHANDLER_H_
#define SRC_COMMON_DEFINITIONHANDLER_H_

#include <lv2/urid/urid.h>
#include <string>

#include "common.h"

namespace cr42y
{

class DefinitionHandler
{
public:
	static DefinitionHandler* getInstance();
	virtual ~DefinitionHandler();

	void load(LV2_URID_Map* map);

	LV2_URID map(std::string uri);

	LV2_URID_Map* thisMap;

	enum msg_types
	{
		osc_update,
		lfo_env_add,
		lfo_env_update,
		lfo_env_remove
	};

	LV2_URID atom_obj;
	LV2_URID atom_int;
	LV2_URID atom_double;

	LV2_URID cr42ynth_uri;
	LV2_URID cr42ynth_ui_uri;
	LV2_URID msg_type;
	LV2_URID msg_key;

private:
	DefinitionHandler();
	static DefinitionHandler* instance;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_DEFINITIONHANDLER_H_ */
