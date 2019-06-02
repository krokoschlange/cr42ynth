#ifndef SRC_DSP_CR42YNTH_H_
#define SRC_DSP_CR42YNTH_H_

#include <vector>

#include "DSPPortCommunicator.h"
#include "Generators/WTOscillator.h"
#include "Generators/LFO.h"
#include "Generators/Envelope.h"

namespace cr42y
{

class CR42Ynth
{
public:
	static CR42Ynth* getInstance();
	virtual ~CR42Ynth();

	float getBPM();

	WTOscillator** getOscillators();
	LFO** getLFOs();
	Envelope** getEnvelopes();

private:
	CR42Ynth();
	static CR42Ynth* instance;

	float bpm;

	DSPPortCommunicator* control;
	DSPPortCommunicator* external;

	WTOscillator** oscillators;
	std::vector<LFO*> lfos;
	std::vector<Envelope*> envelopes;
};

} /* namespace cr42y */

#endif /* SRC_DSP_CR42YNTH_H_ */
