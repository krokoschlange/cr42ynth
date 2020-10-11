#ifndef SRC_UI_OSCSETTINGSCONTROLLER_H_
#define SRC_UI_OSCSETTINGSCONTROLLER_H_

#include <vector>
#include <deque>

namespace cr42y
{
class OscillatorControls;
class WavetableEditData;
class CR42YnthCommunicator;

class OSCSettingsController
{
public:
	OSCSettingsController(CR42YnthCommunicator* comm);
	virtual ~OSCSettingsController();

	WavetableEditData* getEditData(int idx);
	OscillatorControls* getControls(int idx);

private:
	std::vector<WavetableEditData> editData_;
	std::vector<OscillatorControls> controls_;

	std::vector<std::deque<std::pair<char*, std::vector<int>>>> editHistory_;
};

} /* namespace cr42y */

#endif /* SRC_UI_OSCSETTINGSCONTROLLER_H_ */
