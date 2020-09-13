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

#include "rtosc/rtosc.h"

#include "CR42YnthUI.h"

#include <iostream>
#include <fstream>

#include "CR42YnthCommunicator.h"
#include "OSCEvent.h"

//#include "WTEditor.h"
#include "WavetableEditData.h"
#include "WavetableEditController.h"
#include "CR42YTheme.h"

#include "CR42YButton.h"
#include "CR42YLabel.h"
#include "CR42YBoxVScale.h"
#include "CR42YRelativeContainer.h"
#include "CR42YWavetableEditor.h"
#include "CR42YUI.h"

namespace cr42y
{

CR42YnthUI::CR42YnthUI(CR42YnthCommunicator* comm, const char* path) :
		CR42YUI(),
		gtkMain(Gtk::Main::instance()),
		communicator(comm),
		bundlePath(new char[strlen(path) + 1]),
		//dial1(this, 0, 0, 50, 50, "DIAL"),
		wtEditor(nullptr)
{
	set_size_request(500, 350);

	std::ifstream themeConf;
	themeConf.open("theme.conf");
	std::string themeStr;

	if (themeConf.is_open())
	{
		themeConf.seekg(0, std::ios::end);
		themeStr.resize(themeConf.tellg());
		themeConf.seekg(0, std::ios::beg);
		themeConf.read(&themeStr[0], themeStr.size());
		themeConf.close();
	}

	setTheme(new CR42YTheme(themeStr));
	strcpy(bundlePath, path);
	setResourceRoot(bundlePath);

	//wtEditor = new WTEditor(this, 0, 0, 1000, 700, "EDITOR");

	//add(wtEditor);

	//CR42YRelativeContainer* wdgt = new CR42YRelativeContainer(this);

	//test_ = new Gtk::Button("TEST");
	//btn->setText("TEST");
	//test_->signal_clicked().connect(sigc::mem_fun(this, &CR42YnthUI::btnClick));

	//wdgt->put(test_, 0.25, 0.25, 0.5, 0.5);
	//wdgt->add(*test_);

	CR42YWavetableEditor* editor = new CR42YWavetableEditor(this);

	//btn->setText("TEST");
	Cairo::RefPtr<Cairo::Surface> s = Cairo::ImageSurface::create_from_png(resourceRoot() + "media/left.png");
	//btn->setSurfActive(s);

	put(editor, 0, 0, 1, 1);
	editor->show();

	if (comm)
	{
		std::string addr = "/oscillators/0/active";
		uint32_t bufferSize = addr.size() + 32;
		char buffer[bufferSize];
		int len = rtosc_message(buffer, bufferSize, addr.c_str(), "sf", "set_value", 1.0);
		comm->writeMessage(buffer, len, nullptr, 0);

		addr = "/global/state";
		buffer[bufferSize];
		len = rtosc_message(buffer, bufferSize, addr.c_str(), "s", "get");
		comm->writeMessage(buffer, len, nullptr, 0);
	}
}

CR42YnthUI::~CR42YnthUI()
{
	delete[] bundlePath;
	//delete wtEditor;
}

const char* CR42YnthUI::getBundlePath()
{
	return bundlePath;
}

/*int CR42YnthUI::handle(const PuglEvent* event)
 {
 int g = Avtk::Group::handle(event);

 if (event->type == PUGL_BUTTON_RELEASE)
 {
 std::string address = "/oscillators/0/wavetable";

 unsigned int bufferSize = address.size() + 32;
 char buffer[bufferSize];

 int len = rtosc_message(buffer, bufferSize, address.c_str(), "s", "set");

 WavetableEditController* controller = wtEditor->getController();
 if (controller->getData())
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
 ///
 void* dataBuffer = nullptr;
 int dataSize = controller->getData()->getData(&dataBuffer);
 communicator->writeMessage(buffer, len, dataBuffer, dataSize);
 }
 /*else
 {
 communicator->writeMessage(buffer, len, nullptr, 0);
 }//*
 }
 return 1; //if we don't do this, Avtk fucks up and tries to handle the event again :/
 }*/

/*void CR42YnthUI::widgetValueCB(Avtk::Widget* widget)
 {

 }*/

void CR42YnthUI::handleOSCEvent(OSCEvent* event)
{
	
	const char* msg = event->getMessage();
	//communicator->log(msg);
	std::string addr = "/oscillators/0/wavetable";
	char* end = nullptr;
	rtosc_match_path(addr.c_str(), msg, (const char**) &end);
	if (end && *end == '\0')
	{
		//communicator->log(msg);
		if (rtosc_type(msg, 0) == 's' && !strcmp(rtosc_argument(msg, 0).s, "set"))
		{
			if (event->getData())
			{
				//WavetableEditController* controller = wtEditor->getController();
				//controller->setData(new WavetableEditData((char*) event->getData()));
				//wtEditor->requestRedraw();

			}
		}
	}
}

void CR42YnthUI::idle()
{
	gtkMain->iteration(false);
}

CR42YnthCommunicator* CR42YnthUI::getCommunicator()
{
	return communicator;
}

} /* namespace cr42y */
