#ifndef SRC_UI_CR42YOSCPANEL_H_
#define SRC_UI_CR42YOSCPANEL_H_

#include "CR42YGrid.h"

namespace cr42y
{

class CR42YDial;
class CR42YToggle;
class CR42YWFView;

class OSCSettingsController;

class CR42YOSCPanel : public CR42YGrid
{
public:
	CR42YOSCPanel(CR42YUI* ui);
	virtual ~CR42YOSCPanel();

	void connectData(int oscIndex, OSCSettingsController* controller);

private:
	CR42YWFView* wfView_;

	CR42YToggle* oscToggle_;
	CR42YDial* volumeDial_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YOSCPANEL_H_ */
