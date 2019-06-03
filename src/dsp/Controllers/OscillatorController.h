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

#ifndef SRC_DSP_CONTROLLERS_OSCILLATORCONTROLLER_H_
#define SRC_DSP_CONTROLLERS_OSCILLATORCONTROLLER_H_

#include "Controller.h"
#include "../Generators/WTOscillator.h"
#include "../ModulatableDoubleControl.h"
#include "../../common/IntControl.h"
#include "../../common/DoubleControl.h"

namespace cr42y
{

class OscillatorController : public Controller
{
public:
	OscillatorController(Voice* v, WTOscillator* osc, int unisonNumber = 0,
			bool isParent = true);
	virtual ~OscillatorController();

	void nextFrame();

	void setWTPos(float pos);
	void setVolume(float vol);
	void setPan(float p);
	void setFrequencyShift(float fShift);

	float getWTPos();
	float getVolume();
	float getPan();
	float getFrequencyShift();
	float getUnisonSpreadFactor();
private:
	WTOscillator* oscillator;
	float wavePos;

	ModulatableDoubleControl wtPos;
	ModulatableDoubleControl volume;
	ModulatableDoubleControl pan;
	ModulatableDoubleControl frequencyShift;

	OscillatorController** unison;
	float unisonSpreadFactor;
	
};

} /* namespace cr42y */

#endif /* SRC_DSP_CONTROLLERS_OSCILLATORCONTROLLER_H_ */
