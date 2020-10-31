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
#include "OscillatorControls.h"

#include <string>
#include "Control.h"
#include "CR42YnthCommunicator.h"

namespace cr42y
{

OscillatorControls::OscillatorControls(int id, CR42YnthCommunicator* comm) :
		active_("/oscillators/" + std::to_string(id) + "/active", comm),
		smooth_("/oscillators/" + std::to_string(id) + "/smooth", comm),
		noise_("/oscillators/" + std::to_string(id) + "/noise", comm),
		volume_("/oscillators/" + std::to_string(id) + "/volume", comm, 1),
		detune_("/oscillators/" + std::to_string(id) + "/detune", comm, 0, -96,
				96),
		pan_("/oscillators/" + std::to_string(id) + "/pan", comm, 0, -1, 1),
		noteShift_("/oscillators/" + std::to_string(id) + "/note_shift", comm,
				0, -24, 24),
		wtPos_("/oscillators/" + std::to_string(id) + "/wt_pos", comm),
		unisonAmount_("/oscillators/" + std::to_string(id) + "/unison_amount",
				comm, 1, 1, 16),
		unisonDetune_("/oscillators/" + std::to_string(id) + "/unison_detune",
				comm, 0, -8, 8),
		unisonSpread_("/oscillators/" + std::to_string(id) + "/unison_spread",
				comm),
		phaseShift_("/oscillators/" + std::to_string(id) + "/phase_shift", comm,
				0, -1, 1),
		phaseRand_("/oscillators/" + std::to_string(id) + "/phase_rand", comm)
{
	if (id == 0)
	{
		active_.setValue(1, false);
	}
}

OscillatorControls::~OscillatorControls()
{
}

Control* OscillatorControls::getActiveCtrl()
{
	return &active_;
}

Control* OscillatorControls::getSmoothCtrl()
{
	return &smooth_;
}

Control* OscillatorControls::getNoiseCtrl()
{
	return &noise_;
}

Control* OscillatorControls::getVolumeCtrl()
{
	return &volume_;
}

Control* OscillatorControls::getDetuneCtrl()
{
	return &detune_;
}

Control* OscillatorControls::getPanCtrl()
{
	return &pan_;
}

Control* OscillatorControls::getNoteShiftCtrl()
{
	return &noteShift_;
}

Control* OscillatorControls::getWTPosCtrl()
{
	return &wtPos_;
}

Control* OscillatorControls::getUnisonAmountCtrl()
{
	return &unisonAmount_;
}

Control* OscillatorControls::getUnisonDetuneCtrl()
{
	return &unisonDetune_;
}

Control* OscillatorControls::getUnisonSpreadCtrl()
{
	return &unisonSpread_;
}

Control* OscillatorControls::getPhaseShiftCtrl()
{
	return &phaseShift_;
}

Control* OscillatorControls::getPhaseRandCtrl()
{
	return &phaseRand_;
}

} /* namespace cr42y */
