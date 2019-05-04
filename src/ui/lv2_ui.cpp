/*
 * lv2_ui.cpp
 *
 *  Created on: 30.04.2019
 *      Author: fabian
 */

#include <iostream>

#define CR42YnthUIURI "krokoschlange:cr42ynth:ui"

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
		const char * pluginUri, const char * bundlePath,
		LV2UI_Write_Function writeFunction, LV2UI_Controller controller,
		LV2UI_Widget * widget, const LV2_Feature * const * features)
{
	return NULL;
}

static void cleanup(LV2UI_Handle ui)
{

}

static void portEvent(LV2UI_Handle ui,
		uint32_t port,
		uint32_t bufferSize,
		uint32_t format,
		const void* buffer)
{

}

static const void* extensionData(const char* uri)
{
	return NULL;
}


static const LV2UI_Descriptor descriptor =
{
	CR42YnthUIURI,
	instantiate,
	cleanup,
	portEvent,
	extensionData
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	if (index != 0)
	{
		return NULL;
	}
	return &descriptor;

}
