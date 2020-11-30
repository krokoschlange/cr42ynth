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

#ifndef SRC_UI_LV2_CR42YNTHUILV2_H_
#define SRC_UI_LV2_CR42YNTHUILV2_H_

#include <queue>

#include <lv2/ui/ui.h>
#include <lv2/atom/forge.h>
#include <lv2/log/logger.h>
#include <lv2/options/options.h>
#include <utility>

#include "CR42YnthLV2Communicator.h"
#include "CR42YnthUI.h"

namespace cr42y
{

class CR42YnthUI_LV2 : public CR42YnthLV2Communicator
{
public:
	CR42YnthUI_LV2(const char* bundlePath, LV2UI_Write_Function writeFunction,
			LV2UI_Controller ctrler, LV2UI_Widget* widget,
			const LV2_Feature* const * features);
	virtual ~CR42YnthUI_LV2();

	void cleanup();
	void portEvent(uint32_t port, uint32_t bufferSize, uint32_t format,
			const void* buffer);
	
	int idle();

	bool isReady();

	void writeMessage(OSCEvent& event);

	void log(const char* msg);
	
	void scanOption(const LV2_Options_Option* option);
	
	void prepareAtomWrite(size_t atomSize);

protected:
	bool handleOSCEvent(OSCEvent* event);


private:
	bool ready;
	
	uint8_t* forgeBuffer_;

	LV2_Log_Logger* logger;
	LV2UI_Write_Function write;
	LV2UI_Controller controller;
	
	const LV2_Options_Option* lv2Options_;
	
	size_t portMaxSize_;
	size_t portAvailableSpace_;

	CR42YnthUI* ui;
};

} /* namespace cr42y */

#endif /* SRC_UI_LV2_CR42YNTHUILV2_H_ */
