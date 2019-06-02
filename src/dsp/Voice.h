/*
 * Voice.h
 *
 *  Created on: 02.06.2019
 *      Author: fabian
 */

#ifndef SRC_DSP_VOICE_H_
#define SRC_DSP_VOICE_H_

#include "Controllers/OscillatorController.h"
#include "Controllers/LFOController.h"
#include "Controllers/EnvelopeController.h"

namespace cr42y
{

class Voice
{
public:
	Voice();
	virtual ~Voice();

	float nextFrame();

private:
	OscillatorController** oscillators;
	LFOController** lfos;
	EnvelopeController** envelopes;
};

} /* namespace cr42y */

#endif /* SRC_DSP_VOICE_H_ */
