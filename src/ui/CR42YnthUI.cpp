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

#include "rtosc/rtosc.h"

#include "CR42YnthUI.h"

#include "CR42YnthCommunicator.h"
#include "OSCEvent.h"

#include "WTEditor.h"
#include "WavetableEditData.h"

namespace cr42y
{

CR42YnthUI::CR42YnthUI(CR42YnthCommunicator* comm, PuglNativeWindow parent) :
		Avtk::UI(1000, 700, parent, "CR42Ynth"),
		communicator(comm),
		//dial1(this, 0, 0, 50, 50, "DIAL"),
		wtEditor(new WTEditor(this, 0, 0, 1000, 700, "EDITOR"))
{
	add(wtEditor);
	std::string addr = "/oscillators/0/active";
	uint32_t bufferSize = addr.size() + 32;
	char buffer[bufferSize];
	int len = rtosc_message(buffer, bufferSize, addr.c_str(), "sf", "set_value", 1.0);
	comm->writeMessage(buffer, len, nullptr, 0);
}

CR42YnthUI::~CR42YnthUI()
{
}

int CR42YnthUI::handle(const PuglEvent* event)
{
	int g = Avtk::Group::handle(event);
	
	if (event->type == PUGL_BUTTON_RELEASE)
	{
		std::string address = "/oscillators/0/wavetable";

		unsigned int bufferSize = address.size() + 32;
		char buffer[bufferSize];

		int len = rtosc_message(buffer, bufferSize, address.c_str(), "s", "set");

		if (wtEditor->getEditData())
		{
			/*float wtValues[width * height];
			 for (int i = 0; i < height; i++)
			 {
			 for (int j = 0; j < width; j++)
			 {
			 wtValues[i * width + j] = (*wavetable)[i][j];
			 }
			 }
			 CR42YnthDSP::getInstance()->getCommunicator()->writeMessage(buffer, len, (void*) wtValues, width * height * sizeof(float));
			 */
			void* dataBuffer = nullptr;
			int dataSize = wtEditor->getEditData()->getData(&dataBuffer);
			communicator->writeMessage(buffer, len, dataBuffer, dataSize);
		}
		/*else
		{
			communicator->writeMessage(buffer, len, nullptr, 0);
		}*/
	}
	return g;
}

void CR42YnthUI::widgetValueCB(Avtk::Widget* widget)
{

}

void CR42YnthUI::handleOSCEvent(OSCEvent* event)
{
	
	const char* msg = event->getMessage();
	//communicator->log(msg);
	std::string addr = "/oscillators/0/wavetable";
	char* end = nullptr;
	rtosc_match_path(addr.c_str(), msg, (const char**) &end);
	if (end && *end == '\0')
	{
		communicator->log(msg);
		if (rtosc_type(msg, 0) == 's' && !strcmp(rtosc_argument(msg, 0).s, "set"))
		{
			if (event->getData())
			{
				wtEditor->setEditData(new WavetableEditData((char*) event->getData()));
			}
		}
	}
}

} /* namespace cr42y */
