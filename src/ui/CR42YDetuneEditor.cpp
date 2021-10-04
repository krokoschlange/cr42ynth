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

#include "CR42YDetuneEditor.h"

#include "CR42YIntegerEditor.h"

namespace cr42y
{

CR42YDetuneEditor::CR42YDetuneEditor(CR42YUI* ui) :
		CR42YGrid(ui),
		semiEditor_(new CR42YIntegerEditor(ui)),
		centEditor_(new CR42YIntegerEditor(ui)),
		connector_()
{
	using std::placeholders::_1;
	connector_.setWidgetValueSetter((std::function<void(double)>) std::bind(&CR42YDetuneEditor::setValue, this, _1));
	signalChanged().connect(sigc::mem_fun(&connector_, &ControlConnector::setControlValue));
	
	semiEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YDetuneEditor::editorCallback));
	centEditor_->signalChanged().connect(sigc::mem_fun(this, &CR42YDetuneEditor::editorCallback));
	
	semiEditor_->setMin(-48);
	semiEditor_->setMax(48);
	centEditor_->setMin(-100);
	centEditor_->setMax(100);
	
	configureColumn(0, 1, 0, 0, 0, 0);
	
	configureRow(0, 1, 0, 0, 0, 0);
	configureRow(1, 1, 0, 0, 0, 0);
	
	put(semiEditor_, 0, 0);
	put(centEditor_, 1, 0);
}

CR42YDetuneEditor::~CR42YDetuneEditor()
{
	delete semiEditor_;
	delete centEditor_;
}

void CR42YDetuneEditor::connectControl(Control* control)
{
	if (control)
	{
		connector_.connect(*control);
		setValue(connector_.getControl()->getValue());
	}
	else
	{
		setValue(0);
	}
}

void CR42YDetuneEditor::setValue(float value)
{
	int32_t semis = (uint32_t) value;
	int32_t cents = (value - semis) * 100;
	
	semiEditor_->setValue(semis);
	centEditor_->setValue(cents);
	signalChanged_.emit(getValue());
}

float CR42YDetuneEditor::getValue()
{
	return semiEditor_->value() + centEditor_->value() / 100.;
}

sigc::signal<void, float> cr42y::CR42YDetuneEditor::signalChanged()
{
	return signalChanged_;
}

void CR42YDetuneEditor::editorCallback(int)
{
	signalChanged_.emit(getValue());
}

}
