#ifndef SRC_UI_CR42YWFVIEW_H_
#define SRC_UI_CR42YWFVIEW_H_

#include "CR42YGrid.h"

namespace cr42y
{

class CR42YButton;
class CR42YToggleSelector;
class WavetableEditController;
class WavetableEditData;

class CR42YWFView : public CR42YGrid
{
public:
	CR42YWFView(CR42YUI* ui, WavetableEditController* controller, CR42YToggleSelector* viewSelector);
	virtual ~CR42YWFView();

	void setShowAll(bool showAll);

	void setSelectedWaveform(int selectedWaveform);

	void setData(WavetableEditData* data);


protected:
	void on_realize();
	bool on_expose_event(GdkEventExpose* event);

private:
	Glib::RefPtr<Gdk::Window> window_;

	bool showAll_;

	int selectedWaveform_;

	CR42YToggleSelector* viewSelector_;

	WavetableEditData* data_;

	WavetableEditController* controller_;

	CR42YButton* editBtn_;

	void editWFCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWFVIEW_H_ */
