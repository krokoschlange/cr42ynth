#ifndef SRC_UI_CR42YOSCPANELSCROLL_H_
#define SRC_UI_CR42YOSCPANELSCROLL_H_

#include "CR42YGrid.h"

namespace cr42y
{

class CR42YButton;
class CR42YnthCommunicator;
class CR42YOSCPanel;
class OSCSettingsController;

class CR42YOSCPanelScroll : public CR42YGrid
{
public:
	CR42YOSCPanelScroll(CR42YUI* ui, CR42YnthCommunicator* comm);
	virtual ~CR42YOSCPanelScroll();

	void update();

private:
	int panelAmount_ = 3;

	int scrollIndex_;

	OSCSettingsController* controller_;

	std::vector<CR42YOSCPanel*> panels_;
	CR42YButton* leftBtn_;
	CR42YButton* rightBtn_;

	void leftCallback();
	void rightCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YOSCPANELSCROLL_H_ */
