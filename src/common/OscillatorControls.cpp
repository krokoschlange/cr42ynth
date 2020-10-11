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

Control* OscillatorControls::getSmoothCtrl()
{
	return &noise_;
}

Control* OscillatorControls::getVolumeCtrl()
{
	return &volume_;
}

Control* OscillatorControls::getSmoothCtrl()
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
