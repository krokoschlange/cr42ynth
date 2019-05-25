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

#ifndef SRC_DSP_CR42YNTHURIS_H_
#define SRC_DSP_CR42YNTHURIS_H_

#include "lv2/urid/urid.h"

namespace cr42y
{

class CR42YnthURIS
{
public:
	static CR42YnthURIS* getInstance(LV2_URID_Map* m);
	virtual ~CR42YnthURIS();

private:
	CR42YnthURIS(LV2_URID_Map* m);
	static CR42YnthURIS* singleton;
	void setURIDS();

	LV2_URID_Map* map;

	LV2_URID gl_vol;
	LV2_URID gl_pan;

	LV2_URID osc1_enable;
	LV2_URID osc1_vol;
	LV2_URID osc1_pan;
	LV2_URID osc1_wtpos;
	LV2_URID osc1_phase;
	LV2_URID osc1_phase_rand;
	LV2_URID osc1_delta_note;
	LV2_URID osc1_delta_freq;
	LV2_URID osc1_unison_voices;
	LV2_URID osc1_unison_spread;
	LV2_URID osc1_unison_detune;

	LV2_URID osc2_enable;
	LV2_URID osc2_vol;
	LV2_URID osc2_pan;
	LV2_URID osc2_wtpos;
	LV2_URID osc2_phase;
	LV2_URID osc2_phase_rand;
	LV2_URID osc2_delta_note;
	LV2_URID osc2_delta_freq;
	LV2_URID osc2_unison_voices;
	LV2_URID osc2_unison_spread;
	LV2_URID osc2_unison_detune;

	LV2_URID osc3_enable;
	LV2_URID osc3_vol;
	LV2_URID osc3_pan;
	LV2_URID osc3_wtpos;
	LV2_URID osc3_phase;
	LV2_URID osc3_phase_rand;
	LV2_URID osc3_delta_note;
	LV2_URID osc3_delta_freq;
	LV2_URID osc3_unison_voices;
	LV2_URID osc3_unison_spread;
	LV2_URID osc3_unison_detune;

	LV2_URID osc4_enable;
	LV2_URID osc4_vol;
	LV2_URID osc4_pan;
	LV2_URID osc4_wtpos;
	LV2_URID osc4_phase;
	LV2_URID osc4_phase_rand;
	LV2_URID osc4_delta_note;
	LV2_URID osc4_delta_freq;
	LV2_URID osc4_unison_voices;
	LV2_URID osc4_unison_spread;
	LV2_URID osc4_unison_detune;

	LV2_URID osc5_enable;
	LV2_URID osc5_vol;
	LV2_URID osc5_pan;
	LV2_URID osc5_wtpos;
	LV2_URID osc5_phase;
	LV2_URID osc5_phase_rand;
	LV2_URID osc5_delta_note;
	LV2_URID osc5_delta_freq;
	LV2_URID osc5_unison_voices;
	LV2_URID osc5_unison_spread;
	LV2_URID osc5_unison_detune;

	LV2_URID osc6_enable;
	LV2_URID osc6_vol;
	LV2_URID osc6_pan;
	LV2_URID osc6_wtpos;
	LV2_URID osc6_phase;
	LV2_URID osc6_phase_rand;
	LV2_URID osc6_delta_note;
	LV2_URID osc6_delta_freq;
	LV2_URID osc6_unison_voices;
	LV2_URID osc6_unison_spread;
	LV2_URID osc6_unison_detune;

	LV2_URID osc7_enable;
	LV2_URID osc7_vol;
	LV2_URID osc7_pan;
	LV2_URID osc7_wtpos;
	LV2_URID osc7_phase;
	LV2_URID osc7_phase_rand;
	LV2_URID osc7_delta_note;
	LV2_URID osc7_delta_freq;
	LV2_URID osc7_unison_voices;
	LV2_URID osc7_unison_spread;
	LV2_URID osc7_unison_detune;

	LV2_URID osc8_enable;
	LV2_URID osc8_vol;
	LV2_URID osc8_pan;
	LV2_URID osc8_wtpos;
	LV2_URID osc8_phase;
	LV2_URID osc8_phase_rand;
	LV2_URID osc8_delta_note;
	LV2_URID osc8_delta_freq;
	LV2_URID osc8_unison_voices;
	LV2_URID osc8_unison_spread;
	LV2_URID osc8_unison_detune;

	LV2_URID fm_a2b;
	LV2_URID fm_a2c;
	LV2_URID fm_a2d;
	LV2_URID fm_a2e;
	LV2_URID fm_a2f;
	LV2_URID fm_a2g;
	LV2_URID fm_a2h;

	LV2_URID fm_b2c;
	LV2_URID fm_b2d;
	LV2_URID fm_b2e;
	LV2_URID fm_b2f;
	LV2_URID fm_b2g;
	LV2_URID fm_b2h;

	LV2_URID fm_c2d;
	LV2_URID fm_c2e;
	LV2_URID fm_c2f;
	LV2_URID fm_c2g;
	LV2_URID fm_c2h;

	LV2_URID fm_d2e;
	LV2_URID fm_d2f;
	LV2_URID fm_d2g;
	LV2_URID fm_d2h;

	LV2_URID fm_e2f;
	LV2_URID fm_e2g;
	LV2_URID fm_e2h;

	LV2_URID fm_f2g;
	LV2_URID fm_f2h;

	LV2_URID fm_g2h;

	LV2_URID pm_a2b;
	LV2_URID pm_a2c;
	LV2_URID pm_a2d;
	LV2_URID pm_a2e;
	LV2_URID pm_a2f;
	LV2_URID pm_a2g;
	LV2_URID pm_a2h;

	LV2_URID pm_b2c;
	LV2_URID pm_b2d;
	LV2_URID pm_b2e;
	LV2_URID pm_b2f;
	LV2_URID pm_b2g;
	LV2_URID pm_b2h;

	LV2_URID pm_c2d;
	LV2_URID pm_c2e;
	LV2_URID pm_c2f;
	LV2_URID pm_c2g;
	LV2_URID pm_c2h;

	LV2_URID pm_d2e;
	LV2_URID pm_d2f;
	LV2_URID pm_d2g;
	LV2_URID pm_d2h;

	LV2_URID pm_e2f;
	LV2_URID pm_e2g;
	LV2_URID pm_e2h;

	LV2_URID pm_f2g;
	LV2_URID pm_f2h;

	LV2_URID pm_g2h;

	LV2_URID am_a2b;
	LV2_URID am_a2c;
	LV2_URID am_a2d;
	LV2_URID am_a2e;
	LV2_URID am_a2f;
	LV2_URID am_a2g;
	LV2_URID am_a2h;

	LV2_URID am_b2c;
	LV2_URID am_b2d;
	LV2_URID am_b2e;
	LV2_URID am_b2f;
	LV2_URID am_b2g;
	LV2_URID am_b2h;

	LV2_URID am_c2d;
	LV2_URID am_c2e;
	LV2_URID am_c2f;
	LV2_URID am_c2g;
	LV2_URID am_c2h;

	LV2_URID am_d2e;
	LV2_URID am_d2f;
	LV2_URID am_d2g;
	LV2_URID am_d2h;

	LV2_URID am_e2f;
	LV2_URID am_e2g;
	LV2_URID am_e2h;

	LV2_URID am_f2g;
	LV2_URID am_f2h;

	LV2_URID am_g2h;

	LV2_URID rm_a2b;
	LV2_URID rm_a2c;
	LV2_URID rm_a2d;
	LV2_URID rm_a2e;
	LV2_URID rm_a2f;
	LV2_URID rm_a2g;
	LV2_URID rm_a2h;

	LV2_URID rm_b2c;
	LV2_URID rm_b2d;
	LV2_URID rm_b2e;
	LV2_URID rm_b2f;
	LV2_URID rm_b2g;
	LV2_URID rm_b2h;

	LV2_URID rm_c2d;
	LV2_URID rm_c2e;
	LV2_URID rm_c2f;
	LV2_URID rm_c2g;
	LV2_URID rm_c2h;

	LV2_URID rm_d2e;
	LV2_URID rm_d2f;
	LV2_URID rm_d2g;
	LV2_URID rm_d2h;

	LV2_URID rm_e2f;
	LV2_URID rm_e2g;
	LV2_URID rm_e2h;

	LV2_URID rm_f2g;
	LV2_URID rm_f2h;

	LV2_URID rm_g2h;

	LV2_URID ext1;
	LV2_URID ext2;
	LV2_URID ext3;
	LV2_URID ext4;
	LV2_URID ext5;
	LV2_URID ext6;
	LV2_URID ext7;
	LV2_URID ext8;

	LV2_URID new_lfo_env;
	LV2_URID remove_lfo_env;
	LV2_URID update_lfo_env;
	LV2_URID connect_control;
	LV2_URID disconnect_control;
};

} /* namespace cr42y */

#endif /* SRC_DSP_CR42YNTHURIS_H_ */
