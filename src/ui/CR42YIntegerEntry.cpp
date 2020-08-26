/*
 * CR42YIntegerEntry.cpp
 *
 *  Created on: 11.08.2020
 *      Author: fabian
 */

#include "CR42YIntegerEntry.h"

#include "CR42YButton.h"
#include "CR42YEntry.h"

namespace cr42y
{

CR42YIntegerEntry::CR42YIntegerEntry(CR42YUI* ui) :
		Glib::ObjectBase("CR42YIntegerEntry"),
		CR42YRelativeContainer(ui),
		increaseBtn_(nullptr),
		decreaseBtn_(nullptr),
		entry_(nullptr),
		value_(0),
		min_(0),
		max_(8)
{
	increaseBtn_ = new CR42YButton(ui);
	decreaseBtn_ = new CR42YButton(ui);
	entry_ = new CR42YEntry(ui);
	
	Cairo::RefPtr<Cairo::ImageSurface> minus = Cairo::ImageSurface::create_from_png("media/minus.png");
	Cairo::RefPtr<Cairo::ImageSurface> plus = Cairo::ImageSurface::create_from_png("media/plus.png");
	
	increaseBtn_->setSurfActive(plus);
	increaseBtn_->setSurfInactive(plus);
	
	decreaseBtn_->setSurfActive(minus);
	decreaseBtn_->setSurfInactive(minus);
	
	entry_->set_text(std::to_string(value_));
	entry_->setDrawCursor(false);
	entry_->setDrawSelection(false);
	
	increaseBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YIntegerEntry::increaseCallback));
	decreaseBtn_->signalClicked().connect(sigc::mem_fun(this, &CR42YIntegerEntry::decreaseCallback));
	
	entry_->signal_changed().connect(sigc::mem_fun(this, &CR42YIntegerEntry::editCallback));
	entry_->signal_button_press_event().connect(sigc::mem_fun(this, &CR42YIntegerEntry::onButtonPress), false);
	entry_->signal_focus_out_event().connect(sigc::mem_fun(this, &CR42YIntegerEntry::)
	
	put(decreaseBtn_, 0, 0, 0.25, 1);
	put(entry_, 0.25, 0, 0.5, 1);
	put(increaseBtn_, 0.75, 0, 0.25, 1);
}

CR42YIntegerEntry::~CR42YIntegerEntry()
{
	delete increaseBtn_;
	delete decreaseBtn_;
	delete entry_;
}

void CR42YIntegerEntry::setValue(int value)
{
	value_ = std::min<int>(max_, std::max<int>(min_, value));
	valueChangeCallback();
}

int CR42YIntegerEntry::value()
{
	return value_;
}

void CR42YIntegerEntry::editCallback()
{
	Glib::ustring text = entry_->get_text();
	
	
	std::stringstream sstream;
	int newVal = 0;
	
	sstream << text;
	sstream >> newVal;
	
	setValue(newVal);
}

void CR42YIntegerEntry::valueChangeCallback()
{
	entry_->set_text(std::to_string(value_));
	entry_->set_position(entry_->get_text().size());
}

void CR42YIntegerEntry::increaseCallback()
{
	setValue(value() + 1);
}

void CR42YIntegerEntry::decreaseCallback()
{
	setValue(value() - 1);
}

bool CR42YIntegerEntry::onButtonPress(GdkEventButton* event)
{
	entry_->set_position(entry_->get_text().size());
	return false;
}

} /* namespace cr42y */
