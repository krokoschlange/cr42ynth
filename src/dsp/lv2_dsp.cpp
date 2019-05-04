/*
 * lv2_dsp.cpp
 *
 *  Created on: 01.05.2019
 *      Author: fabian
 */

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define CR42YnthURI "krokoschlange:cr42ynth"

static const LV2_Descriptor descriptor = {
		CR42YnthURI,

};


LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	if (index != 0)
	{
		return NULL;
	}
	return &descriptor;

}
