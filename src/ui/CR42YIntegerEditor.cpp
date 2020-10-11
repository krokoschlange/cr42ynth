/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 * 3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior
 *     written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "CR42YIntegerEditor.h"
#include "CR42YButton.h"
#include "CR42YIntegerEntry.h"
#include "CR42YUI.h"

namespace cr42y
{

CR42YIntegerEditor::CR42YIntegerEditor(CR42YUI* ui) :
		Glib::ObjectBase("CR42YIntegerEditor"),
		CR42YRelativeContainer(ui)
{
	setDrawBG(false);
	setDrawBorder(false);

	increaseBtn_ = new CR42YButton(ui);
	decreaseBtn_ = new CR42YButton(ui);
	entry_ = new CR42YIntegerEntry(ui);
	
	Cairo::RefPtr<Cairo::ImageSurface> minus = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/minus.png");
	Cairo::RefPtr<Cairo::ImageSurface> plus = Cairo::ImageSurface::create_from_png(ui->resourceRoot() + "media/plus.png");

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

void CR42YIntegerEditor::setFontSize(int fontSize)
{
	entry_->setFontSize(fontSize);
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
