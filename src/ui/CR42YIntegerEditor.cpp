/*
 * CR42YIntegerEditor.cpp
 *
 *  Created on: 29.08.2020
 *      Author: fabian
 */

#include "CR42YIntegerEditor.h"
#include "CR42YButton.h"
#include "CR42YIntegerEntry.h"

namespace cr42y
{

CR42YIntegerEditor::CR42YIntegerEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YIntegerEditor"),
		CR42YRelativeContainer(ui)
{
	setDrawBG(false);
	setDrawBorder(true);

	increaseBtn_ = new CR42YButton(ui);
	decreaseBtn_ = new CR42YButton(ui);
	entry_ = new CR42YIntegerEntry(ui);
	
	Cairo::RefPtr<Cairo::ImageSurface> minus = Cairo::ImageSurface::create_from_png("media/minus.png");
	Cairo::RefPtr<Cairo::ImageSurface> plus = Cairo::ImageSurface::create_from_png("media/plus.png");

	increaseBtn_->setSurfActive(plus);
	increaseBtn_->setSurfInactive(plus);

	decreaseBtn_->setSurfActive(minus);
	decreaseBtn_->setSurfInactive(minus);

	increaseBtn_->setAutorepeat(true);
	decreaseBtn_->setAutorepeat(true);

	increaseBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YIntegerEditor::increaseCallback));
	decreaseBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YIntegerEditor::decreaseCallback));

	put(decreaseBtn_, 0, 0, 0.25, 1);
	put(entry_, 0.25, 0, 0.5, 1);
	put(increaseBtn_, 0.75, 0, 0.25, 1);
}

CR42YIntegerEditor::~CR42YIntegerEditor()
{
	delete increaseBtn_;
	delete decreaseBtn_;
	delete entry_;
}

void CR42YIntegerEditor::setValue(int value)
{
	entry_->setValue(value);
}

int CR42YIntegerEditor::value()
{
	return entry_->value();
}

void CR42YIntegerEditor::setMin(int min, bool use)
{
	entry_->setMin(min, use);
}

int CR42YIntegerEditor::min()
{
	return entry_->min();
}

void CR42YIntegerEditor::setMax(int max, bool use)
{
	entry_->setMax(max, use);
}

int CR42YIntegerEditor::max()
{
	return entry_->max();
}

sigc::signal<void, int> CR42YIntegerEditor::signalChanged()
{
	return entry_->signalChanged();
}

void CR42YIntegerEditor::increaseCallback()
{
	entry_->setValue(entry_->value() + 1);
}

void CR42YIntegerEditor::decreaseCallback()
{
	entry_->setValue(entry_->value() - 1);
}

} /* namespace cr42y */
