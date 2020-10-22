#ifndef SRC_UI_CR42YOSCPANEL_H_
#define SRC_UI_CR42YOSCPANEL_H_

#include "CR42YGrid.h"
#include "ControlListener.h"

namespace cr42y
{

class CR42YControlDial;
class CR42YLabel;
class CR42YToggle;
class CR42YToggleSelector;
class CR42YWFView;

class WavetableEditController;

class OSCSettingsController;

class CR42YOSCPanel : public CR42YGrid
{
public:
	CR42YOSCPanel(CR42YUI* ui, WavetableEditController* wtEditController, CR42YToggleSelector* viewSelector);
	virtual ~CR42YOSCPanel();

	void connectData(int oscIndex, OSCSettingsController* controller);

private:
	CR42YWFView* wfView_;

	CR42YLabel* idxLabel_;

	CR42YToggle* oscToggle_;
	CR42YControlDial* volumeDial_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YOSCPANEL_H_ */
