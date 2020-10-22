#ifndef SRC_UI_OSCSETTINGSCONTROLLER_H_
#define SRC_UI_OSCSETTINGSCONTROLLER_H_

#include "OSCEventListener.h"

#include <vector>
#include <deque>

namespace cr42y
{
class OscillatorControls;
class WavetableEditData;
class CR42YnthCommunicator;
class WavetableEditController;

class OSCSettingsController : OSCEventListener
{
public:
	OSCSettingsController(CR42YnthCommunicator* communicator, WavetableEditController* wtEditController);
	virtual ~OSCSettingsController();

	WavetableEditData* getEditData(int idx);
	OscillatorControls* getControls(int idx);

	bool handleOSCEvent(OSCEvent* event);
	void sendState();

	void wavetableChangedCallback();

	void eraseHistoryCallback(std::deque<std::pair<char*, std::vector<int>>>);

private:
	CR42YnthCommunicator* communicator_;
	WavetableEditController* wtEditController_;

	std::vector<WavetableEditData> editData_;
	std::vector<OscillatorControls> controls_;

	std::vector<std::deque<std::pair<char*, std::vector<int>>>> editHistory_;
};

} /* namespace cr42y */

#endif /* SRC_UI_OSCSETTINGSCONTROLLER_H_ */
