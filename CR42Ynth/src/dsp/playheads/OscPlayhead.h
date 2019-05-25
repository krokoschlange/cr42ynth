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

#ifndef SRC_DSP_OSCVOICE_H_
#define SRC_DSP_OSCVOICE_H_

#include "WTOscillator.h"
#include "Control.h"

namespace cr42y
{
class WTOscillator;

class OscPlayhead
{
public:
	OscPlayhead(WTOscillator* osc, float phase, float n, float vol, float wtp,
			float p);
	virtual ~OscPlayhead();

	void nextSample();

	float getValue();
	float getOutput();

	float getNote();
	float getPhase();
	void movePhase(float newPhase);

	void setDeltaPhase(float p);
	float getDeltaPhase();

	void setDeltaFreq(float f);
	float getDeltaFreq();

	void setVol(float v);
	float getVol();

	void setWTPos(float pos);
	float getWTPos();

	void setPan(float p);
	float getPan();

	void setFM(float f);
	float getFM();
	void setAM(float a);
	float getAM();
	void setPM(float p);
	float getPM();
	void setRM(float r);
	float getRM();


private:
	WTOscillator* oscillator;

	float lastPos;
	float deltaPhaseLFO_ENV;

	float note;
	float deltaFrequencyLFO_ENV;

	float volume;

	float wtPos;
	float pan;

	float value;
	float output;

	float FM;
	float AM;
	float PM;
	float RM;

	Control** controls;
};

} /* namespace cr42y */

#endif /* SRC_DSP_OSCVOICE_H_ */
