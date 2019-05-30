
#include "Envelope.h"

namespace cr42y
{

Envelope::Envelope(float rate, PortCommunicator* comm) :
		samplerate(rate),
		lenght(1, comm) //TODO
{

}

Envelope::~Envelope()
{
}

float Envelope::getSample(float* pos)
{
	if (!waveform)
	{
		*pos += 1 / samplerate;
		return 0;
	}
	if (*pos > lenght.getValue())
	{
		return 0;
	}

	float out;
	float waveSample = (*pos / lenght.getValue()) * waveform->size();
	if (smooth)
	{
		float smpl1 = (*waveform)[(int) waveSample];
		float smpl2;
		if (waveSample + 1 < waveform->size())
		{
			smpl2 = (*waveform)[(int) waveSample + 1];
		}
		else
		{
			smpl2 = 0;
		}

		out = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
	}
	else
	{
		out = (*waveform)[(int) waveSample];
	}

	*pos += 1 / samplerate;
	return out;
}

void Envelope::setEnvelope(std::vector<float>* env, float len)
{
	if (waveform)
	{
		delete waveform;
	}
	waveform = env;
	lenght = len;
}

void Envelope::setLength(float len)
{
	lenght = len;
}

float Envelope::getLenght()
{
	return lenght;
}

} /* namespace cr42y */
