#include "OSCSettingsController.h"

#include "common.h"

namespace cr42y
{

OSCSettingsController::OSCSettingsController(CR42YnthCommunicator* comm)
{
	editData_.resize(CR42Ynth_OSC_COUNT, WavetableEditData());
	controls_.resize(CR42Ynth_OSC_COUNT);
	editHistory_.resize(CR42Ynth_OSC_COUNT, std::deque<std::pair<char*, std::vector<int>>>());

	for (int i = 0; i < controls_.size(); i++)
	{
		controls_[i] = OscillatorControls(i, comm);
	}
}

OSCSettingsController::~OSCSettingsController()
{
	for (int i = 0; i < editHistory_.size(); i++)
	{
		for (int j = 0; j < editHistory_[i].size(); j++)
		{
			delete[] editHistory_[i][j].first;
		}
		editHistory_[i].clear();
	}
	editHistory_.clear();
}

WavetableEditData* OSCSettingsController::getEditData(int idx)
{
	if (idx >= 0 && idx < editData_.size())
	{
		return &(editData_[idx]);
	}
	return nullptr;
}

OscillatorControls* OSCSettingsController::getControls(int idx)
{
	if (idx >= 0 && idx < controls_.size())
	{
		return &(controls_[idx]);
	}
	return nullptr;
}

} /* namespace cr42y */
