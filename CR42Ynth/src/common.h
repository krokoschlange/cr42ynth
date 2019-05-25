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

#include "lv2/urid/urid.h"

#define CR42YnthURI "https://github.com/krokoschlange/cr42ynth"
#define CR42YnthUIURI "https://github.com/krokoschlange/cr42ynth#ui"

#define CR42YnthCTRL CR42YnthURI "/ctrl#"

#define Ctrl_GL_VOL CR42YnthCTRL "gl_vol"
#define Ctrl_GL_PAN CR42YnthCTRL "gl_pan"

#define Ctrl_OSC1_ENABLE CR42YnthCTRL "osc1_enable"
#define Ctrl_OSC1_VOL CR42YnthCTRL "osc1_vol"
#define Ctrl_OSC1_PAN CR42YnthCTRL "osc1_pan"
#define Ctrl_OSC1_WTPOS CR42YnthCTRL "osc1_wtpos"
#define Ctrl_OSC1_PHASE CR42YnthCTRL "osc1_phase"
#define Ctrl_OSC1_PHASE_RAND CR42YnthCTRL "osc1_phase_rand"
#define Ctrl_OSC1_DELTA_NOTE CR42YnthCTRL "osc1_delta_note"
#define Ctrl_OSC1_DELTA_FREQ CR42YnthCTRL "osc1_delta_freq"
#define Ctrl_OSC1_UNISON_VOICES CR42YnthCTRL "osc1_unison_voices"
#define Ctrl_OSC1_UNISON_SPREAD CR42YnthCTRL "osc1_unison_spread"
#define Ctrl_OSC1_UNISON_DETUNE CR42YnthCTRL "osc1_unison_detune"

#define Ctrl_OSC2_ENABLE CR42YnthCTRL "osc2_enable"
#define Ctrl_OSC2_VOL CR42YnthCTRL "osc2_vol"
#define Ctrl_OSC2_PAN CR42YnthCTRL "osc2_pan"
#define Ctrl_OSC2_WTPOS CR42YnthCTRL "osc2_wtpos"
#define Ctrl_OSC2_PHASE CR42YnthCTRL "osc2_phase"
#define Ctrl_OSC2_PHASE_RAND CR42YnthCTRL "osc2_phase_rand"
#define Ctrl_OSC2_DELTA_NOTE CR42YnthCTRL "osc2_delta_note"
#define Ctrl_OSC2_DELTA_FREQ CR42YnthCTRL "osc2_delta_freq"
#define Ctrl_OSC2_UNISON_VOICES CR42YnthCTRL "osc2_unison_voices"
#define Ctrl_OSC2_UNISON_SPREAD CR42YnthCTRL "osc2_unison_spread"
#define Ctrl_OSC2_UNISON_DETUNE CR42YnthCTRL "osc2_unison_detune"

#define Ctrl_OSC3_ENABLE CR42YnthCTRL "osc3_enable"
#define Ctrl_OSC3_VOL CR42YnthCTRL "osc3_vol"
#define Ctrl_OSC3_PAN CR42YnthCTRL "osc3_pan"
#define Ctrl_OSC3_WTPOS CR42YnthCTRL "osc3_wtpos"
#define Ctrl_OSC3_PHASE CR42YnthCTRL "osc3_phase"
#define Ctrl_OSC3_PHASE_RAND CR42YnthCTRL "osc3_phase_rand"
#define Ctrl_OSC3_DELTA_NOTE CR42YnthCTRL "osc3_delta_note"
#define Ctrl_OSC3_DELTA_FREQ CR42YnthCTRL "osc3_delta_freq"
#define Ctrl_OSC3_UNISON_VOICES CR42YnthCTRL "osc3_unison_voices"
#define Ctrl_OSC3_UNISON_SPREAD CR42YnthCTRL "osc3_unison_spread"
#define Ctrl_OSC3_UNISON_DETUNE CR42YnthCTRL "osc3_unison_detune"

#define Ctrl_OSC4_ENABLE CR42YnthCTRL "osc4_enable"
#define Ctrl_OSC4_VOL CR42YnthCTRL "osc4_vol"
#define Ctrl_OSC4_PAN CR42YnthCTRL "osc4_pan"
#define Ctrl_OSC4_WTPOS CR42YnthCTRL "osc4_wtpos"
#define Ctrl_OSC4_PHASE CR42YnthCTRL "osc4_phase"
#define Ctrl_OSC4_PHASE_RAND CR42YnthCTRL "osc4_phase_rand"
#define Ctrl_OSC4_DELTA_NOTE CR42YnthCTRL "osc4_delta_note"
#define Ctrl_OSC4_DELTA_FREQ CR42YnthCTRL "osc4_delta_freq"
#define Ctrl_OSC4_UNISON_VOICES CR42YnthCTRL "osc4_unison_voices"
#define Ctrl_OSC4_UNISON_SPREAD CR42YnthCTRL "osc4_unison_spread"
#define Ctrl_OSC4_UNISON_DETUNE CR42YnthCTRL "osc4_unison_detune"

#define Ctrl_OSC5_ENABLE CR42YnthCTRL "osc5_enable"
#define Ctrl_OSC5_VOL CR42YnthCTRL "osc5_vol"
#define Ctrl_OSC5_PAN CR42YnthCTRL "osc5_pan"
#define Ctrl_OSC5_WTPOS CR42YnthCTRL "osc5_wtpos"
#define Ctrl_OSC5_PHASE CR42YnthCTRL "osc5_phase"
#define Ctrl_OSC5_PHASE_RAND CR42YnthCTRL "osc5_phase_rand"
#define Ctrl_OSC5_DELTA_NOTE CR42YnthCTRL "osc5_delta_note"
#define Ctrl_OSC5_DELTA_FREQ CR42YnthCTRL "osc5_delta_freq"
#define Ctrl_OSC5_UNISON_VOICES CR42YnthCTRL "osc5_unison_voices"
#define Ctrl_OSC5_UNISON_SPREAD CR42YnthCTRL "osc5_unison_spread"
#define Ctrl_OSC5_UNISON_DETUNE CR42YnthCTRL "osc5_unison_detune"

#define Ctrl_OSC6_ENABLE CR42YnthCTRL "osc6_enable"
#define Ctrl_OSC6_VOL CR42YnthCTRL "osc6_vol"
#define Ctrl_OSC6_PAN CR42YnthCTRL "osc6_pan"
#define Ctrl_OSC6_WTPOS CR42YnthCTRL "osc6_wtpos"
#define Ctrl_OSC6_PHASE CR42YnthCTRL "osc6_phase"
#define Ctrl_OSC6_PHASE_RAND CR42YnthCTRL "osc6_phase_rand"
#define Ctrl_OSC6_DELTA_NOTE CR42YnthCTRL "osc6_delta_note"
#define Ctrl_OSC6_DELTA_FREQ CR42YnthCTRL "osc6_delta_freq"
#define Ctrl_OSC6_UNISON_VOICES CR42YnthCTRL "osc6_unison_voices"
#define Ctrl_OSC6_UNISON_SPREAD CR42YnthCTRL "osc6_unison_spread"
#define Ctrl_OSC6_UNISON_DETUNE CR42YnthCTRL "osc6_unison_detune"

#define Ctrl_OSC7_ENABLE CR42YnthCTRL "osc7_enable"
#define Ctrl_OSC7_VOL CR42YnthCTRL "osc7_vol"
#define Ctrl_OSC7_PAN CR42YnthCTRL "osc7_pan"
#define Ctrl_OSC7_WTPOS CR42YnthCTRL "osc7_wtpos"
#define Ctrl_OSC7_PHASE CR42YnthCTRL "osc7_phase"
#define Ctrl_OSC7_PHASE_RAND CR42YnthCTRL "osc7_phase_rand"
#define Ctrl_OSC7_DELTA_NOTE CR42YnthCTRL "osc7_delta_note"
#define Ctrl_OSC7_DELTA_FREQ CR42YnthCTRL "osc7_delta_freq"
#define Ctrl_OSC7_UNISON_VOICES CR42YnthCTRL "osc7_unison_voices"
#define Ctrl_OSC7_UNISON_SPREAD CR42YnthCTRL "osc7_unison_spread"
#define Ctrl_OSC7_UNISON_DETUNE CR42YnthCTRL "osc7_unison_detune"

#define Ctrl_OSC8_ENABLE CR42YnthCTRL "osc8_enable"
#define Ctrl_OSC8_VOL CR42YnthCTRL "osc8_vol"
#define Ctrl_OSC8_PAN CR42YnthCTRL "osc8_pan"
#define Ctrl_OSC8_WTPOS CR42YnthCTRL "osc8_wtpos"
#define Ctrl_OSC8_PHASE CR42YnthCTRL "osc8_phase"
#define Ctrl_OSC8_PHASE_RAND CR42YnthCTRL "osc8_phase_rand"
#define Ctrl_OSC8_DELTA_NOTE CR42YnthCTRL "osc8_delta_note"
#define Ctrl_OSC8_DELTA_FREQ CR42YnthCTRL "osc8_delta_freq"
#define Ctrl_OSC8_UNISON_VOICES CR42YnthCTRL "osc8_unison_voices"
#define Ctrl_OSC8_UNISON_SPREAD CR42YnthCTRL "osc8_unison_spread"
#define Ctrl_OSC8_UNISON_DETUNE CR42YnthCTRL "osc8_unison_detune"



#define Ctrl_FM_A2B CR42YnthCTRL "fm_a2b"
#define Ctrl_FM_A2C CR42YnthCTRL "fm_a2c"
#define Ctrl_FM_A2D CR42YnthCTRL "fm_a2d"
#define Ctrl_FM_A2E CR42YnthCTRL "fm_a2e"
#define Ctrl_FM_A2F CR42YnthCTRL "fm_a2f"
#define Ctrl_FM_A2G CR42YnthCTRL "fm_a2g"
#define Ctrl_FM_A2H CR42YnthCTRL "fm_a2h"

#define Ctrl_FM_B2C CR42YnthCTRL "fm_b2c"
#define Ctrl_FM_B2D CR42YnthCTRL "fm_b2d"
#define Ctrl_FM_B2E CR42YnthCTRL "fm_b2e"
#define Ctrl_FM_B2F CR42YnthCTRL "fm_b2f"
#define Ctrl_FM_B2G CR42YnthCTRL "fm_b2g"
#define Ctrl_FM_B2H CR42YnthCTRL "fm_b2h"

#define Ctrl_FM_C2D CR42YnthCTRL "fm_c2d"
#define Ctrl_FM_C2E CR42YnthCTRL "fm_c2e"
#define Ctrl_FM_C2F CR42YnthCTRL "fm_c2f"
#define Ctrl_FM_C2G CR42YnthCTRL "fm_c2g"
#define Ctrl_FM_C2H CR42YnthCTRL "fm_c2h"

#define Ctrl_FM_D2E CR42YnthCTRL "fm_d2e"
#define Ctrl_FM_D2F CR42YnthCTRL "fm_d2f"
#define Ctrl_FM_D2G CR42YnthCTRL "fm_d2g"
#define Ctrl_FM_D2H CR42YnthCTRL "fm_d2h"

#define Ctrl_FM_E2F CR42YnthCTRL "fm_e2f"
#define Ctrl_FM_E2G CR42YnthCTRL "fm_e2g"
#define Ctrl_FM_E2H CR42YnthCTRL "fm_e2h"

#define Ctrl_FM_F2G CR42YnthCTRL "fm_f2g"
#define Ctrl_FM_F2H CR42YnthCTRL "fm_f2h"

#define Ctrl_FM_G2H CR42YnthCTRL "fm_g2h"



#define Ctrl_PM_A2B CR42YnthCTRL "pm_a2b"
#define Ctrl_PM_A2C CR42YnthCTRL "pm_a2c"
#define Ctrl_PM_A2D CR42YnthCTRL "pm_a2d"
#define Ctrl_PM_A2E CR42YnthCTRL "pm_a2e"
#define Ctrl_PM_A2F CR42YnthCTRL "pm_a2f"
#define Ctrl_PM_A2G CR42YnthCTRL "pm_a2g"
#define Ctrl_PM_A2H CR42YnthCTRL "pm_a2h"

#define Ctrl_PM_B2C CR42YnthCTRL "pm_b2c"
#define Ctrl_PM_B2D CR42YnthCTRL "pm_b2d"
#define Ctrl_PM_B2E CR42YnthCTRL "pm_b2e"
#define Ctrl_PM_B2F CR42YnthCTRL "pm_b2f"
#define Ctrl_PM_B2G CR42YnthCTRL "pm_b2g"
#define Ctrl_PM_B2H CR42YnthCTRL "pm_b2h"

#define Ctrl_PM_C2D CR42YnthCTRL "pm_c2d"
#define Ctrl_PM_C2E CR42YnthCTRL "pm_c2e"
#define Ctrl_PM_C2F CR42YnthCTRL "pm_c2f"
#define Ctrl_PM_C2G CR42YnthCTRL "pm_c2g"
#define Ctrl_PM_C2H CR42YnthCTRL "pm_c2h"

#define Ctrl_PM_D2E CR42YnthCTRL "pm_d2e"
#define Ctrl_PM_D2F CR42YnthCTRL "pm_d2f"
#define Ctrl_PM_D2G CR42YnthCTRL "pm_d2g"
#define Ctrl_PM_D2H CR42YnthCTRL "pm_d2h"

#define Ctrl_PM_E2F CR42YnthCTRL "pm_e2f"
#define Ctrl_PM_E2G CR42YnthCTRL "pm_e2g"
#define Ctrl_PM_E2H CR42YnthCTRL "pm_e2h"

#define Ctrl_PM_F2G CR42YnthCTRL "pm_f2g"
#define Ctrl_PM_F2H CR42YnthCTRL "pm_f2h"

#define Ctrl_PM_G2H CR42YnthCTRL "pm_g2h"



#define Ctrl_AM_A2B CR42YnthCTRL "am_a2b"
#define Ctrl_AM_A2C CR42YnthCTRL "am_a2c"
#define Ctrl_AM_A2D CR42YnthCTRL "am_a2d"
#define Ctrl_AM_A2E CR42YnthCTRL "am_a2e"
#define Ctrl_AM_A2F CR42YnthCTRL "am_a2f"
#define Ctrl_AM_A2G CR42YnthCTRL "am_a2g"
#define Ctrl_AM_A2H CR42YnthCTRL "am_a2h"

#define Ctrl_AM_B2C CR42YnthCTRL "am_b2c"
#define Ctrl_AM_B2D CR42YnthCTRL "am_b2d"
#define Ctrl_AM_B2E CR42YnthCTRL "am_b2e"
#define Ctrl_AM_B2F CR42YnthCTRL "am_b2f"
#define Ctrl_AM_B2G CR42YnthCTRL "am_b2g"
#define Ctrl_AM_B2H CR42YnthCTRL "am_b2h"

#define Ctrl_AM_C2D CR42YnthCTRL "am_c2d"
#define Ctrl_AM_C2E CR42YnthCTRL "am_c2e"
#define Ctrl_AM_C2F CR42YnthCTRL "am_c2f"
#define Ctrl_AM_C2G CR42YnthCTRL "am_c2g"
#define Ctrl_AM_C2H CR42YnthCTRL "am_c2h"

#define Ctrl_AM_D2E CR42YnthCTRL "am_d2e"
#define Ctrl_AM_D2F CR42YnthCTRL "am_d2f"
#define Ctrl_AM_D2G CR42YnthCTRL "am_d2g"
#define Ctrl_AM_D2H CR42YnthCTRL "am_d2h"

#define Ctrl_AM_E2F CR42YnthCTRL "am_e2f"
#define Ctrl_AM_E2G CR42YnthCTRL "am_e2g"
#define Ctrl_AM_E2H CR42YnthCTRL "am_e2h"

#define Ctrl_AM_F2G CR42YnthCTRL "am_f2g"
#define Ctrl_AM_F2H CR42YnthCTRL "am_f2h"

#define Ctrl_AM_G2H CR42YnthCTRL "am_g2h"



#define Ctrl_RM_A2B CR42YnthCTRL "rm_a2b"
#define Ctrl_RM_A2C CR42YnthCTRL "rm_a2c"
#define Ctrl_RM_A2D CR42YnthCTRL "rm_a2d"
#define Ctrl_RM_A2E CR42YnthCTRL "rm_a2e"
#define Ctrl_RM_A2F CR42YnthCTRL "rm_a2f"
#define Ctrl_RM_A2G CR42YnthCTRL "rm_a2g"
#define Ctrl_RM_A2H CR42YnthCTRL "rm_a2h"

#define Ctrl_RM_B2C CR42YnthCTRL "rm_b2c"
#define Ctrl_RM_B2D CR42YnthCTRL "rm_b2d"
#define Ctrl_RM_B2E CR42YnthCTRL "rm_b2e"
#define Ctrl_RM_B2F CR42YnthCTRL "rm_b2f"
#define Ctrl_RM_B2G CR42YnthCTRL "rm_b2g"
#define Ctrl_RM_B2H CR42YnthCTRL "rm_b2h"

#define Ctrl_RM_C2D CR42YnthCTRL "rm_c2d"
#define Ctrl_RM_C2E CR42YnthCTRL "rm_c2e"
#define Ctrl_RM_C2F CR42YnthCTRL "rm_c2f"
#define Ctrl_RM_C2G CR42YnthCTRL "rm_c2g"
#define Ctrl_RM_C2H CR42YnthCTRL "rm_c2h"

#define Ctrl_RM_D2E CR42YnthCTRL "rm_d2e"
#define Ctrl_RM_D2F CR42YnthCTRL "rm_d2f"
#define Ctrl_RM_D2G CR42YnthCTRL "rm_d2g"
#define Ctrl_RM_D2H CR42YnthCTRL "rm_d2h"

#define Ctrl_RM_E2F CR42YnthCTRL "rm_e2f"
#define Ctrl_RM_E2G CR42YnthCTRL "rm_e2g"
#define Ctrl_RM_E2H CR42YnthCTRL "rm_e2h"

#define Ctrl_RM_F2G CR42YnthCTRL "rm_f2g"
#define Ctrl_RM_F2H CR42YnthCTRL "rm_f2h"

#define Ctrl_RM_G2H CR42YnthCTRL "rm_g2h"



#define Ctrl_EXT1 CR42YnthCTRL "ext1"
#define Ctrl_EXT2 CR42YnthCTRL "ext2"
#define Ctrl_EXT3 CR42YnthCTRL "ext3"
#define Ctrl_EXT4 CR42YnthCTRL "ext4"
#define Ctrl_EXT5 CR42YnthCTRL "ext5"
#define Ctrl_EXT6 CR42YnthCTRL "ext6"
#define Ctrl_EXT7 CR42YnthCTRL "ext7"
#define Ctrl_EXT8 CR42YnthCTRL "ext8"



#define New_LFO_ENV CR42YnthURI "#new_lfo_env"
#define Remove_LFO_ENV CR42YnthURI "#remove_lfo_env"
#define Update_LFO_ENV CR42YnthURI "#update_lfo_env"

#define Connect_CONTROL "#connect_control"
#define Disconnect_CONTROL "#disconnect_control"


#endif /* COMMON_H_ */
