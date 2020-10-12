#ifndef SRC_COMMON_OSCILLATORCONTROLS_H_
#define SRC_COMMON_OSCILLATORCONTROLS_H_

#include "Control.h"

namespace cr42y
{

class Control;
class OSCEvent;
class CR42YnthCommunicator;

class OscillatorControls
{
public:
	OscillatorControls(int id, CR42YnthCommunicator* comm);
	virtual ~OscillatorControls();

	bool receiveOSCMessage(OSCEvent* event);

	Control* getActiveCtrl();
	Control* getSmoothCtrl();
	Control* getNoiseCtrl();
	Control* getVolumeCtrl();
	Control* getDetuneCtrl();
	Control* getPanCtrl();
	Control* getNoteShiftCtrl();
	Control* getWTPosCtrl();
	Control* getUnisonAmountCtrl();
	Control* getUnisonDetuneCtrl();
	Control* getUnisonSpreadCtrl();
	Control* getPhaseShiftCtrl();
	Control* getPhaseRandCtrl();

private:
	Control active_;
	Control smooth_;
	Control noise_;
	Control volume_;
	Control detune_;
	Control pan_;
	Control noteShift_;
	Control wtPos_;
	Control unisonAmount_;
	Control unisonDetune_;
	Control unisonSpread_;
	Control phaseShift_;
	Control phaseRand_;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_OSCILLATORCONTROLS_H_ */
