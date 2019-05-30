
#ifndef SRC_DSP_GENERATORS_ENVELOPE_H_
#define SRC_DSP_GENERATORS_ENVELOPE_H_

#include <vector>

#include "../../common/DoubleControl.h"
#include "../../common/BoolControl.h"

namespace cr42y
{

class Envelope
{
public:
	Envelope(float rate, PortCommunicator* comm);
	virtual ~Envelope();

	float getSample(float* pos);

	void setEnvelope(std::vector<float>* env, float len);

	void setLength(float len);

	float getLenght();

private:
	float samplerate;
	std::vector<float>* waveform;
	DoubleControl lenght;
	BoolControl smooth;
};

} /* namespace cr42y */

#endif /* SRC_DSP_GENERATORS_ENVELOPE_H_ */
