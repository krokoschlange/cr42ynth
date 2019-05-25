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

#ifndef SRC_UI_CR42YNTHUI_H_
#define SRC_UI_CR42YNTHUI_H_

#include "avtk.hxx"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

namespace cr42y
{

class CR42YnthUI: public Avtk::UI
{
public:
	CR42YnthUI(PuglNativeWindow parent);
	virtual ~CR42YnthUI();

	static LV2UI_Handle lv2Instantiate(
			const struct _LV2UI_Descriptor* descriptor, const char* pluginUri,
			const char* bundlePath, LV2UI_Write_Function writeFunction,
			LV2UI_Controller controller, LV2UI_Widget* widget,
			const LV2_Feature* const * features);

	static void lv2Cleanup(LV2UI_Handle handle);
	static void lv2PortEvent(LV2UI_Handle handle, uint32_t portIndex,
			uint32_t bufferSize, uint32_t format, const void* buffer);
	static int lv2Idle(LV2UI_Handle handle);
	static const void* lv2ExtensionData(const char* uri);

	void cleanup();
	void portEvent(uint32_t portIndex, uint32_t bufferSize, uint32_t format,
			const void* buffer);
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YNTHUI_H_ */
