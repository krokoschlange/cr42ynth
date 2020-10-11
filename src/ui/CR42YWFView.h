#ifndef SRC_UI_CR42YWFVIEW_H_
#define SRC_UI_CR42YWFVIEW_H_

#include "CR42YGrid.h"

namespace cr42y
{

class CR42YButton;

class CR42YWFView : public CR42YGrid
{
public:
	CR42YWFView(CR42YUI* ui);
	virtual ~CR42YWFView();

protected:
	void on_expose_event(GdkEventExpose* event);

private:
	bool showAll_;

	CR42YButton* editBtn_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWFVIEW_H_ */
