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

#include "CR42YnthURIS.h"
#include "../common.h"

namespace cr42y
{
CR42YnthURIS* CR42YnthURIS::singleton;

CR42YnthURIS* CR42YnthURIS::getInstance(LV2_URID_Map* m)
{
	if (singleton == nullptr)
	{
		singleton = new CR42YnthURIS(m);
	}
	return singleton;
}

CR42YnthURIS::CR42YnthURIS(LV2_URID_Map* m)
{
	map = m;
	setURIDS();
}

CR42YnthURIS::~CR42YnthURIS()
{
}

void CR42YnthURIS::setURIDS()
{
	gl_vol = map->map(map->handle, Ctrl_GL_VOL);
	gl_pan = map->map(map->handle, Ctrl_GL_PAN);

	osc1_enable = map->map(map->handle, Ctrl_OSC1_ENABLE);
	osc1_vol = map->map(map->handle, Ctrl_OSC1_VOL);
	osc1_pan = map->map(map->handle, Ctrl_OSC1_PAN);
	osc1_wtpos = map->map(map->handle, Ctrl_OSC1_WTPOS);
	osc1_phase = map->map(map->handle, Ctrl_OSC1_PHASE);
	osc1_phase_rand = map->map(map->handle, Ctrl_OSC1_PHASE_RAND);
	osc1_delta_note = map->map(map->handle, Ctrl_OSC1_DELTA_NOTE);
	osc1_delta_freq = map->map(map->handle, Ctrl_OSC1_DELTA_FREQ);
	osc1_unison_voices = map->map(map->handle, Ctrl_OSC1_UNISON_VOICES);
	osc1_unison_spread = map->map(map->handle, Ctrl_OSC1_UNISON_SPREAD);
	osc1_unison_detune = map->map(map->handle, Ctrl_OSC1_UNISON_DETUNE);

	osc2_enable = map->map(map->handle, Ctrl_OSC2_ENABLE);
	osc2_vol = map->map(map->handle, Ctrl_OSC2_VOL);
	osc2_pan = map->map(map->handle, Ctrl_OSC2_PAN);
	osc2_wtpos = map->map(map->handle, Ctrl_OSC2_WTPOS);
	osc2_phase = map->map(map->handle, Ctrl_OSC2_PHASE);
	osc2_phase_rand = map->map(map->handle, Ctrl_OSC2_PHASE_RAND);
	osc2_delta_note = map->map(map->handle, Ctrl_OSC2_DELTA_NOTE);
	osc2_delta_freq = map->map(map->handle, Ctrl_OSC2_DELTA_FREQ);
	osc2_unison_voices = map->map(map->handle, Ctrl_OSC2_UNISON_VOICES);
	osc2_unison_spread = map->map(map->handle, Ctrl_OSC2_UNISON_SPREAD);
	osc2_unison_detune = map->map(map->handle, Ctrl_OSC2_UNISON_DETUNE);

	osc3_enable = map->map(map->handle, Ctrl_OSC3_ENABLE);
	osc3_vol = map->map(map->handle, Ctrl_OSC3_VOL);
	osc3_pan = map->map(map->handle, Ctrl_OSC3_PAN);
	osc3_wtpos = map->map(map->handle, Ctrl_OSC3_WTPOS);
	osc3_phase = map->map(map->handle, Ctrl_OSC3_PHASE);
	osc3_phase_rand = map->map(map->handle, Ctrl_OSC3_PHASE_RAND);
	osc3_delta_note = map->map(map->handle, Ctrl_OSC3_DELTA_NOTE);
	osc3_delta_freq = map->map(map->handle, Ctrl_OSC3_DELTA_FREQ);
	osc3_unison_voices = map->map(map->handle, Ctrl_OSC3_UNISON_VOICES);
	osc3_unison_spread = map->map(map->handle, Ctrl_OSC3_UNISON_SPREAD);
	osc3_unison_detune = map->map(map->handle, Ctrl_OSC3_UNISON_DETUNE);

	osc4_enable = map->map(map->handle, Ctrl_OSC4_ENABLE);
	osc4_vol = map->map(map->handle, Ctrl_OSC4_VOL);
	osc4_pan = map->map(map->handle, Ctrl_OSC4_PAN);
	osc4_wtpos = map->map(map->handle, Ctrl_OSC4_WTPOS);
	osc4_phase = map->map(map->handle, Ctrl_OSC4_PHASE);
	osc4_phase_rand = map->map(map->handle, Ctrl_OSC4_PHASE_RAND);
	osc4_delta_note = map->map(map->handle, Ctrl_OSC4_DELTA_NOTE);
	osc4_delta_freq = map->map(map->handle, Ctrl_OSC4_DELTA_FREQ);
	osc4_unison_voices = map->map(map->handle, Ctrl_OSC4_UNISON_VOICES);
	osc4_unison_spread = map->map(map->handle, Ctrl_OSC4_UNISON_SPREAD);
	osc4_unison_detune = map->map(map->handle, Ctrl_OSC4_UNISON_DETUNE);

	osc5_enable = map->map(map->handle, Ctrl_OSC5_ENABLE);
	osc5_vol = map->map(map->handle, Ctrl_OSC5_VOL);
	osc5_pan = map->map(map->handle, Ctrl_OSC5_PAN);
	osc5_wtpos = map->map(map->handle, Ctrl_OSC5_WTPOS);
	osc5_phase = map->map(map->handle, Ctrl_OSC5_PHASE);
	osc5_phase_rand = map->map(map->handle, Ctrl_OSC5_PHASE_RAND);
	osc5_delta_note = map->map(map->handle, Ctrl_OSC5_DELTA_NOTE);
	osc5_delta_freq = map->map(map->handle, Ctrl_OSC5_DELTA_FREQ);
	osc5_unison_voices = map->map(map->handle, Ctrl_OSC5_UNISON_VOICES);
	osc5_unison_spread = map->map(map->handle, Ctrl_OSC5_UNISON_SPREAD);
	osc5_unison_detune = map->map(map->handle, Ctrl_OSC5_UNISON_DETUNE);

	osc6_enable = map->map(map->handle, Ctrl_OSC6_ENABLE);
	osc6_vol = map->map(map->handle, Ctrl_OSC6_VOL);
	osc6_pan = map->map(map->handle, Ctrl_OSC6_PAN);
	osc6_wtpos = map->map(map->handle, Ctrl_OSC6_WTPOS);
	osc6_phase = map->map(map->handle, Ctrl_OSC6_PHASE);
	osc6_phase_rand = map->map(map->handle, Ctrl_OSC6_PHASE_RAND);
	osc6_delta_note = map->map(map->handle, Ctrl_OSC6_DELTA_NOTE);
	osc6_delta_freq = map->map(map->handle, Ctrl_OSC6_DELTA_FREQ);
	osc6_unison_voices = map->map(map->handle, Ctrl_OSC6_UNISON_VOICES);
	osc6_unison_spread = map->map(map->handle, Ctrl_OSC6_UNISON_SPREAD);
	osc6_unison_detune = map->map(map->handle, Ctrl_OSC6_UNISON_DETUNE);

	osc7_enable = map->map(map->handle, Ctrl_OSC7_ENABLE);
	osc7_vol = map->map(map->handle, Ctrl_OSC7_VOL);
	osc7_pan = map->map(map->handle, Ctrl_OSC7_PAN);
	osc7_wtpos = map->map(map->handle, Ctrl_OSC7_WTPOS);
	osc7_phase = map->map(map->handle, Ctrl_OSC7_PHASE);
	osc7_phase_rand = map->map(map->handle, Ctrl_OSC7_PHASE_RAND);
	osc7_delta_note = map->map(map->handle, Ctrl_OSC7_DELTA_NOTE);
	osc7_delta_freq = map->map(map->handle, Ctrl_OSC7_DELTA_FREQ);
	osc7_unison_voices = map->map(map->handle, Ctrl_OSC7_UNISON_VOICES);
	osc7_unison_spread = map->map(map->handle, Ctrl_OSC7_UNISON_SPREAD);
	osc7_unison_detune = map->map(map->handle, Ctrl_OSC7_UNISON_DETUNE);

	osc8_enable = map->map(map->handle, Ctrl_OSC8_ENABLE);
	osc8_vol = map->map(map->handle, Ctrl_OSC8_VOL);
	osc8_pan = map->map(map->handle, Ctrl_OSC8_PAN);
	osc8_wtpos = map->map(map->handle, Ctrl_OSC8_WTPOS);
	osc8_phase = map->map(map->handle, Ctrl_OSC8_PHASE);
	osc8_phase_rand = map->map(map->handle, Ctrl_OSC8_PHASE_RAND);
	osc8_delta_note = map->map(map->handle, Ctrl_OSC8_DELTA_NOTE);
	osc8_delta_freq = map->map(map->handle, Ctrl_OSC8_DELTA_FREQ);
	osc8_unison_voices = map->map(map->handle, Ctrl_OSC8_UNISON_VOICES);
	osc8_unison_spread = map->map(map->handle, Ctrl_OSC8_UNISON_SPREAD);
	osc8_unison_detune = map->map(map->handle, Ctrl_OSC8_UNISON_DETUNE);



	fm_a2b = map->map(map->handle, Ctrl_FM_A2B);
	fm_a2c = map->map(map->handle, Ctrl_FM_A2C);
	fm_a2d = map->map(map->handle, Ctrl_FM_A2D);
	fm_a2e = map->map(map->handle, Ctrl_FM_A2E);
	fm_a2f = map->map(map->handle, Ctrl_FM_A2F);
	fm_a2g = map->map(map->handle, Ctrl_FM_A2G);
	fm_a2h = map->map(map->handle, Ctrl_FM_A2H);

	fm_b2c = map->map(map->handle, Ctrl_FM_B2C);
	fm_b2d = map->map(map->handle, Ctrl_FM_B2D);
	fm_b2e = map->map(map->handle, Ctrl_FM_B2E);
	fm_b2f = map->map(map->handle, Ctrl_FM_B2F);
	fm_b2g = map->map(map->handle, Ctrl_FM_B2G);
	fm_b2h = map->map(map->handle, Ctrl_FM_B2H);

	fm_c2d = map->map(map->handle, Ctrl_FM_C2D);
	fm_c2e = map->map(map->handle, Ctrl_FM_C2E);
	fm_c2f = map->map(map->handle, Ctrl_FM_C2F);
	fm_c2g = map->map(map->handle, Ctrl_FM_C2G);
	fm_c2h = map->map(map->handle, Ctrl_FM_C2H);

	fm_d2e = map->map(map->handle, Ctrl_FM_D2E);
	fm_d2f = map->map(map->handle, Ctrl_FM_D2F);
	fm_d2g = map->map(map->handle, Ctrl_FM_D2G);
	fm_d2h = map->map(map->handle, Ctrl_FM_D2H);

	fm_e2f = map->map(map->handle, Ctrl_FM_E2F);
	fm_e2g = map->map(map->handle, Ctrl_FM_E2G);
	fm_e2h = map->map(map->handle, Ctrl_FM_E2H);

	fm_f2g = map->map(map->handle, Ctrl_FM_F2G);
	fm_f2h = map->map(map->handle, Ctrl_FM_F2H);

	fm_g2h = map->map(map->handle, Ctrl_FM_G2H);



	pm_a2b = map->map(map->handle, Ctrl_PM_A2B);
	pm_a2c = map->map(map->handle, Ctrl_PM_A2C);
	pm_a2d = map->map(map->handle, Ctrl_PM_A2D);
	pm_a2e = map->map(map->handle, Ctrl_PM_A2E);
	pm_a2f = map->map(map->handle, Ctrl_PM_A2F);
	pm_a2g = map->map(map->handle, Ctrl_PM_A2G);
	pm_a2h = map->map(map->handle, Ctrl_PM_A2H);

	pm_b2c = map->map(map->handle, Ctrl_PM_B2C);
	pm_b2d = map->map(map->handle, Ctrl_PM_B2D);
	pm_b2e = map->map(map->handle, Ctrl_PM_B2E);
	pm_b2f = map->map(map->handle, Ctrl_PM_B2F);
	pm_b2g = map->map(map->handle, Ctrl_PM_B2G);
	pm_b2h = map->map(map->handle, Ctrl_PM_B2H);

	pm_c2d = map->map(map->handle, Ctrl_PM_C2D);
	pm_c2e = map->map(map->handle, Ctrl_PM_C2E);
	pm_c2f = map->map(map->handle, Ctrl_PM_C2F);
	pm_c2g = map->map(map->handle, Ctrl_PM_C2G);
	pm_c2h = map->map(map->handle, Ctrl_PM_C2H);

	pm_d2e = map->map(map->handle, Ctrl_PM_D2E);
	pm_d2f = map->map(map->handle, Ctrl_PM_D2F);
	pm_d2g = map->map(map->handle, Ctrl_PM_D2G);
	pm_d2h = map->map(map->handle, Ctrl_PM_D2H);

	pm_e2f = map->map(map->handle, Ctrl_PM_E2F);
	pm_e2g = map->map(map->handle, Ctrl_PM_E2G);
	pm_e2h = map->map(map->handle, Ctrl_PM_E2H);

	pm_f2g = map->map(map->handle, Ctrl_PM_F2G);
	pm_f2h = map->map(map->handle, Ctrl_PM_F2H);

	pm_g2h = map->map(map->handle, Ctrl_PM_G2H);



	am_a2b = map->map(map->handle, Ctrl_AM_A2B);
	am_a2c = map->map(map->handle, Ctrl_AM_A2C);
	am_a2d = map->map(map->handle, Ctrl_AM_A2D);
	am_a2e = map->map(map->handle, Ctrl_AM_A2E);
	am_a2f = map->map(map->handle, Ctrl_AM_A2F);
	am_a2g = map->map(map->handle, Ctrl_AM_A2G);
	am_a2h = map->map(map->handle, Ctrl_AM_A2H);

	am_b2c = map->map(map->handle, Ctrl_AM_B2C);
	am_b2d = map->map(map->handle, Ctrl_AM_B2D);
	am_b2e = map->map(map->handle, Ctrl_AM_B2E);
	am_b2f = map->map(map->handle, Ctrl_AM_B2F);
	am_b2g = map->map(map->handle, Ctrl_AM_B2G);
	am_b2h = map->map(map->handle, Ctrl_AM_B2H);

	am_c2d = map->map(map->handle, Ctrl_AM_C2D);
	am_c2e = map->map(map->handle, Ctrl_AM_C2E);
	am_c2f = map->map(map->handle, Ctrl_AM_C2F);
	am_c2g = map->map(map->handle, Ctrl_AM_C2G);
	am_c2h = map->map(map->handle, Ctrl_AM_C2H);

	am_d2e = map->map(map->handle, Ctrl_AM_D2E);
	am_d2f = map->map(map->handle, Ctrl_AM_D2F);
	am_d2g = map->map(map->handle, Ctrl_AM_D2G);
	am_d2h = map->map(map->handle, Ctrl_AM_D2H);

	am_e2f = map->map(map->handle, Ctrl_AM_E2F);
	am_e2g = map->map(map->handle, Ctrl_AM_E2G);
	am_e2h = map->map(map->handle, Ctrl_AM_E2H);

	am_f2g = map->map(map->handle, Ctrl_AM_F2G);
	am_f2h = map->map(map->handle, Ctrl_AM_F2H);

	am_g2h = map->map(map->handle, Ctrl_AM_G2H);



	rm_a2b = map->map(map->handle, Ctrl_RM_A2B);
	rm_a2c = map->map(map->handle, Ctrl_RM_A2C);
	rm_a2d = map->map(map->handle, Ctrl_RM_A2D);
	rm_a2e = map->map(map->handle, Ctrl_RM_A2E);
	rm_a2f = map->map(map->handle, Ctrl_RM_A2F);
	rm_a2g = map->map(map->handle, Ctrl_RM_A2G);
	rm_a2h = map->map(map->handle, Ctrl_RM_A2H);

	rm_b2c = map->map(map->handle, Ctrl_RM_B2C);
	rm_b2d = map->map(map->handle, Ctrl_RM_B2D);
	rm_b2e = map->map(map->handle, Ctrl_RM_B2E);
	rm_b2f = map->map(map->handle, Ctrl_RM_B2F);
	rm_b2g = map->map(map->handle, Ctrl_RM_B2G);
	rm_b2h = map->map(map->handle, Ctrl_RM_B2H);

	rm_c2d = map->map(map->handle, Ctrl_RM_C2D);
	rm_c2e = map->map(map->handle, Ctrl_RM_C2E);
	rm_c2f = map->map(map->handle, Ctrl_RM_C2F);
	rm_c2g = map->map(map->handle, Ctrl_RM_C2G);
	rm_c2h = map->map(map->handle, Ctrl_RM_C2H);

	rm_d2e = map->map(map->handle, Ctrl_RM_D2E);
	rm_d2f = map->map(map->handle, Ctrl_RM_D2F);
	rm_d2g = map->map(map->handle, Ctrl_RM_D2G);
	rm_d2h = map->map(map->handle, Ctrl_RM_D2H);

	rm_e2f = map->map(map->handle, Ctrl_RM_E2F);
	rm_e2g = map->map(map->handle, Ctrl_RM_E2G);
	rm_e2h = map->map(map->handle, Ctrl_RM_E2H);

	rm_f2g = map->map(map->handle, Ctrl_RM_F2G);
	rm_f2h = map->map(map->handle, Ctrl_RM_F2H);

	rm_g2h = map->map(map->handle, Ctrl_RM_G2H);



	ext1 = map->map(map->handle, Ctrl_EXT1);
	ext2 = map->map(map->handle, Ctrl_EXT2);
	ext3 = map->map(map->handle, Ctrl_EXT3);
	ext4 = map->map(map->handle, Ctrl_EXT4);
	ext5 = map->map(map->handle, Ctrl_EXT5);
	ext6 = map->map(map->handle, Ctrl_EXT6);
	ext7 = map->map(map->handle, Ctrl_EXT7);
	ext8 = map->map(map->handle, Ctrl_EXT8);



	new_lfo_env = map->map(map->handle, New_LFO_ENV);
	remove_lfo_env = map->map(map->handle, Remove_LFO_ENV);
	update_lfo_env = map->map(map->handle, Update_LFO_ENV);

	connect_control = map->map(map->handle, Connect_CONTROL);
	disconnect_control = map->map(map->handle, Disconnect_CONTROL);

}

} /* namespace cr42y */
