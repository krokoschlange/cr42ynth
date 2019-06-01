#ifndef SRC_DSP_CR42YNTH_H_
#define SRC_DSP_CR42YNTH_H_

#include "DSPPortCommunicator.h"

namespace cr42y
{

class CR42Ynth
{
public:
	static CR42Ynth* getInstance();
	virtual ~CR42Ynth();

	float getBPM();

private:
	CR42Ynth();
	static CR42Ynth* instance;

	float bpm;

	DSPPortCommunicator* control;
	DSPPortCommunicator* external;
};

} /* namespace cr42y */

#endif /* SRC_DSP_CR42YNTH_H_ */
