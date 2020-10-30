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

#include <fstream>

#include "CR42YnthCommunicator.h"
#include "OSCEvent.h"

#include "CR42YTheme.h"

#include "CR42YWavetableEditor.h"
#include "CR42YToggleSelector.h"
#include "CR42YToggle.h"
#include "CR42YOSCSettings.h"

namespace cr42y
{

CR42YnthUI::CR42YnthUI(CR42YnthCommunicator* comm, const char* path) :
		Glib::ObjectBase("CR42YnthUI"),
		CR42YUI(),
		gtkMain_(Gtk::Main::instance()),
		communicator_(comm),
		bundlePath_(new char[strlen(path) + 1]),
		screenSelector_(nullptr),
		oscSettings_(nullptr),
		wtEditor_(nullptr),
		selectedScreen_(-1)
{
	set_size_request(500, 350);

	strcpy(bundlePath_, path);
	setResourceRoot(std::string(bundlePath_) + "../");

	std::ifstream themeConf;
	themeConf.open(resourceRoot() + "theme.conf");
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

	screenSelector_ = new CR42YToggleSelector(this);
	wtEditor_ = new CR42YWavetableEditor(this);
	oscSettings_ = new CR42YOSCSettings(this, comm, wtEditor_->getController(), screenSelector_);


	CR42YToggle* tgl = new CR42YToggle(this);
	tgl->setText("OSC");
	tgl->setFontSize(CR42YTheme::BIG);
	screenSelector_->putToggle(tgl, 0, 0, 0.25, 1, 2, 2, 0, 2);
	tgl = new CR42YToggle(this);
	tgl->setText("MOD");
	tgl->setFontSize(CR42YTheme::BIG);
	screenSelector_->putToggle(tgl, 0.25, 0, 0.25, 1, 0, 2, 0, 2);
	tgl = new CR42YToggle(this);
	tgl->setText("FX");
	tgl->setFontSize(CR42YTheme::BIG);
	screenSelector_->putToggle(tgl, 0.5, 0, 0.25, 1, 0, 2, 0, 2);
	tgl = new CR42YToggle(this);
	tgl->setText("WT");
	tgl->setFontSize(CR42YTheme::BIG);
	screenSelector_->putToggle(tgl, 0.75, 0, 0.25, 1, 0, 2, 2, 2);

	screenSelector_->select(0);

	screenSelector_->signalSelected().connect(
			sigc::mem_fun(this, &CR42YnthUI::screenSelectCallback));

	put(screenSelector_, 0.25, 0, 0.5, 0.15);
	put(oscSettings_, 0, 0.15, 1, 0.85);
	put(wtEditor_, 0, 0.15, 1, 0.85);

	if (comm)
	{
		std::string addr = "/oscillators/0/active";
		uint32_t bufferSize = addr.size() + 32;
		char buffer[bufferSize];
		int len = rtosc_message(buffer, bufferSize, addr.c_str(), "sf",
				"set_value", 1.0);
		OSCEvent event = OSCEvent(buffer, len, nullptr, 0);
		comm->writeMessage(event);

		addr = "/global/state";
		buffer[bufferSize];
		len = rtosc_message(buffer, bufferSize, addr.c_str(), "s", "get");
		event = OSCEvent(buffer, len, nullptr, 0);
		comm->writeMessage(event);
	}
}

CR42YnthUI::~CR42YnthUI()
{
	delete[] bundlePath_;
	//delete wtEditor;
}

const char* CR42YnthUI::getBundlePath()
{
	return bundlePath_;
}

void CR42YnthUI::idle()
{
	gtkMain_->iteration(false);
}

CR42YnthCommunicator* CR42YnthUI::getCommunicator()
{
	return communicator_;
}

void CR42YnthUI::on_realize()
{
	CR42YUI::on_realize();
	screenSelectCallback(0);
}

void CR42YnthUI::on_show()
{
	CR42YUI::on_show();
	screenSelectCallback(0);
}

void CR42YnthUI::screenSelectCallback(int selected)
{
	selectedScreen_ = selected;

	oscSettings_->hide_all();
	wtEditor_->hide_all();

	switch (selected)
	{
	case 0:
		oscSettings_->show_all();
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		wtEditor_->show_all();
		break;
	default:
		break;
	}
}

} /* namespace cr42y */
