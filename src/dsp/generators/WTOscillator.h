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

#ifndef SRC_DSP_WTOSCILLATOR_H_
#define SRC_DSP_WTOSCILLATOR_H_

#include <vector>
#include "Control.h"
#include "OscPlayhead.h"

namespace cr42y
{
class OscPlayhead;

class WTOscillator
{
public:
	WTOscillator(float rate, int id);
	virtual ~WTOscillator();

	float getSample(OscPlayhead* voice);

	void setWavetable(std::vector<std::vector<float>>* wt)
	{
		delete wavetable;
		wavetable = wt;
	}

	void setSmooth(bool s)
	{
		smooth = s;
	}

	const int getID()
	{
		return ID;
	}

	float getDetune();
	void setDetune(float d);
	void setDetune(int semi, int cents);

	void setEnabled(bool state);
	bool getEnabled();

private:
	const int ID;

	std::vector<std::vector<float>>* wavetable;
	float samplerate;

	bool smooth;

	float detune;

	bool enabled;
};

} /* namespace cr42y */

#endif /* SRC_DSP_WTOSCILLATOR_H_ */
