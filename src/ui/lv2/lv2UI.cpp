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

#include <iostream>
#include <lv2/ui/ui.h>
#include <lv2/options/options.h>

#include "common.h"
#include "lv2_common.h"
#include "CR42YnthUILV2.h"

#include <gtkmm.h>
#include "CR42YnthUI.h"

namespace cr42y
{

LV2UI_Handle instantiateUI(const LV2UI_Descriptor* descriptor,
		const char* plugin_uri, const char* bundlePath,
		LV2UI_Write_Function writeFunction, LV2UI_Controller controller,
		LV2UI_Widget* widget, const LV2_Feature* const * features)
{
	CR42YnthUI_LV2* ui = new CR42YnthUI_LV2(bundlePath, writeFunction, controller, widget, features);
	if (ui->isReady())
	{
		return (LV2_Handle) ui;
	}
	return nullptr;
}

void cleanupUI(LV2UI_Handle handle)
{
	CR42YnthUI_LV2* ui = (CR42YnthUI_LV2*) handle;
	delete ui;
	ui = nullptr;
}

void portEventUI(LV2UI_Handle handle, uint32_t port, uint32_t bufferSize,
		uint32_t format, const void* buffer)
{
	CR42YnthUI_LV2* ui = (CR42YnthUI_LV2*) handle;
	ui->portEvent(port, bufferSize, format, buffer);
}

int idleUI(LV2UI_Handle handle)
{
	CR42YnthUI_LV2* ui = (CR42YnthUI_LV2*) handle;
	return ui->idle();
}

static const LV2UI_Idle_Interface idleIface =
	{idleUI};

uint32_t getOptions(LV2_Handle handle, LV2_Options_Option* options)
{
	return LV2_OPTIONS_SUCCESS;
}

uint32_t setOptions(LV2_Handle handle, const LV2_Options_Option* options)
{
	CR42YnthUI_LV2* ui = (CR42YnthUI_LV2*) handle;
	for (const LV2_Options_Option* op = options; op; op++)
	{
		ui->scanOption(op);
	}
	return LV2_OPTIONS_SUCCESS;
}

static const LV2_Options_Interface optionsIface =
	{getOptions, setOptions};

const void* extensionDataUI(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface))
	{
		return &idleIface;
	}
	else if (!strcmp(uri, LV2_OPTIONS__interface))
	{
		return &optionsIface;
	}
	return nullptr;
}

static const LV2UI_Descriptor descriptorUI =
	{CR42Ynth__UI, instantiateUI, cleanupUI, portEventUI, extensionDataUI};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index)
	{
	case 0:
		return &descriptorUI;
	default:
		return nullptr;
	}
}
} /* namespace cr42y */

int main(int argc, char* argv[])
{
	std::cout << "Use as LV2 Plugin\n";

	Gtk::Main* main = new Gtk::Main(argc, argv);

	Gtk::Window win(Gtk::WINDOW_TOPLEVEL);

	cr42y::CR42YnthUI* ui = new cr42y::CR42YnthUI(nullptr, "media/");

	win.add(*ui);
	win.show_all();
	win.present();
	//main->run(*ui);
	while (win.is_visible())
	{
		main->iteration(false);
		Glib::usleep(100);
	}

	return 0;
}
