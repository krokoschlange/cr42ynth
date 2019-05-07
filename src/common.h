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

#ifndef COMMON_H_
#define COMMON_H_

#define CR42YnthURI "https://github.com/krokoschlange/cr42ynth"
#define CR42YnthUIURI "https://github.com/krokoschlange/cr42ynth#ui"

namespace cr42y
{
enum ControlIDs
{
	GL_VOL = 0,
	GL_PAN,

	OSC_1_VOL,
	OSC_1_FREQ,
	OSC_1_PAN,
	OSC_1_WTPOS,
	OSC_1_UNISON_DETUNE,
	OSC_1_UNISON_SPREAD,

	OSC_2_VOL,
	OSC_2_FREQ,
	OSC_2_PAN,
	OSC_2_WTPOS,
	OSC_2_UNISON_DETUNE,
	OSC_2_UNISON_SPREAD,

	OSC_3_VOL,
	OSC_3_FREQ,
	OSC_3_PAN,
	OSC_3_WTPOS,
	OSC_3_UNISON_DETUNE,
	OSC_3_UNISON_SPREAD,

	OSC_4_VOL,
	OSC_4_FREQ,
	OSC_4_PAN,
	OSC_4_WTPOS,
	OSC_4_UNISON_DETUNE,
	OSC_4_UNISON_SPREAD,

	OSC_5_VOL,
	OSC_5_FREQ,
	OSC_5_PAN,
	OSC_5_WTPOS,
	OSC_5_UNISON_DETUNE,
	OSC_5_UNISON_SPREAD,

	OSC_6_VOL,
	OSC_6_FREQ,
	OSC_6_PAN,
	OSC_6_WTPOS,
	OSC_6_UNISON_DETUNE,
	OSC_6_UNISON_SPREAD,

	OSC_7_VOL,
	OSC_7_FREQ,
	OSC_7_PAN,
	OSC_7_WTPOS,
	OSC_7_UNISON_DETUNE,
	OSC_7_UNISON_SPREAD,

	OSC_8_VOL,
	OSC_8_FREQ,
	OSC_8_PAN,
	OSC_8_WTPOS,
	OSC_8_UNISON_DETUNE,
	OSC_8_UNISON_SPREAD,

	FM_A2B,
	FM_A2C,
	FM_A2D,
	FM_A2E,
	FM_A2F,
	FM_A2G,
	FM_A2H,

	FM_B2C,
	FM_B2D,
	FM_B2E,
	FM_B2F,
	FM_B2G,
	FM_B2H,

	FM_C2D,
	FM_C2E,
	FM_C2F,
	FM_C2G,
	FM_C2H,

	FM_D2E,
	FM_D2F,
	FM_D2G,
	FM_D2H,

	FM_E2F,
	FM_E2G,
	FM_E2H,

	FM_F2G,
	FM_F2H,

	FM_G2H,

	PM_A2B,
	PM_A2C,
	PM_A2D,
	PM_A2E,
	PM_A2F,
	PM_A2G,
	PM_A2H,

	PM_B2C,
	PM_B2D,
	PM_B2E,
	PM_B2F,
	PM_B2G,
	PM_B2H,

	PM_C2D,
	PM_C2E,
	PM_C2F,
	PM_C2G,
	PM_C2H,

	PM_D2E,
	PM_D2F,
	PM_D2G,
	PM_D2H,

	PM_E2F,
	PM_E2G,
	PM_E2H,

	PM_F2G,
	PM_F2H,

	PM_G2H,


	AM_A2B,
	AM_A2C,
	AM_A2D,
	AM_A2E,
	AM_A2F,
	AM_A2G,
	AM_A2H,

	AM_B2C,
	AM_B2D,
	AM_B2E,
	AM_B2F,
	AM_B2G,
	AM_B2H,

	AM_C2D,
	AM_C2E,
	AM_C2F,
	AM_C2G,
	AM_C2H,

	AM_D2E,
	AM_D2F,
	AM_D2G,
	AM_D2H,

	AM_E2F,
	AM_E2G,
	AM_E2H,

	AM_F2G,
	AM_F2H,

	AM_G2H,


	RM_A2B,
	RM_A2C,
	RM_A2D,
	RM_A2E,
	RM_A2F,
	RM_A2G,
	RM_A2H,

	RM_B2C,
	RM_B2D,
	RM_B2E,
	RM_B2F,
	RM_B2G,
	RM_B2H,

	RM_C2D,
	RM_C2E,
	RM_C2F,
	RM_C2G,
	RM_C2H,

	RM_D2E,
	RM_D2F,
	RM_D2G,
	RM_D2H,

	RM_E2F,
	RM_E2G,
	RM_E2H,

	RM_F2G,
	RM_F2H,

	RM_G2H

};

}

#endif /* COMMON_H_ */
