#include "CR42Ynth.h"

namespace cr42y
{

CR42Ynth* CR42Ynth::instance;
CR42Ynth::CR42Ynth() :
				bpm(140)
{
}

CR42Ynth* CR42Ynth::getInstance()
{
	if (!instance)
	{
		instance = new CR42Ynth();
	}
	return instance;
}

CR42Ynth::~CR42Ynth()
{
}

float CR42Ynth::getBPM()
{
	return bpm;
}

} /* namespace cr42y */
