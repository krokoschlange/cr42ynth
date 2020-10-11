#include "CR42YWFView.h"

#include "CR42YButton.h"

namespace cr42y
{

CR42YWFView::CR42YWFView(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWFView"),
		CR42YGrid(ui),
		showAll_(false),
		editBtn_(new CR42YButton(ui))
{
	configureColumn(0, 10, 0, 0, 0, 0);
	configureColumn(1, 1, 0.003, 0.003, 0, 0);

	configureRow(0, 1, 0.003, 0.003, 0, 0);
	configureRow(1, 7, 0, 0, 0, 0);


	editBtn_->setText("Edit");
	put(editBtn_, 0, 1);
}

CR42YWFView::~CR42YWFView()
{
}



} /* namespace cr42y */
