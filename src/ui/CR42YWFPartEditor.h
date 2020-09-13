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
/*
 * CR42YWFPartEditor.h
 *
 *  Created on: 05.09.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWFPARTEDITOR_H_
#define SRC_UI_CR42YWFPARTEDITOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class WavetableEditController;
class CR42YLabel;
class CR42YEntry;
class CR42YIntegerEditor;
class CR42YToggleSelector;
class CR42YToggle;

class CR42YWFPartEditor : public CR42YRelativeContainer
{
public:
	CR42YWFPartEditor(CR42YUI* ui);
	virtual ~CR42YWFPartEditor();

	void setController(WavetableEditController* controller);

	void update();

private:
	WavetableEditController* controller_;

	CR42YLabel* wfPartPosLabel_;
	CR42YIntegerEditor* wfPartPosEditor_;

	CR42YToggleSelector* typeSelector_;

	int currentType_;

	CR42YRelativeContainer* wfFunctionGroup_;
	CR42YLabel* wfFunctionLabel_;
	CR42YEntry* wfFunctionEditor_;

	CR42YRelativeContainer* wfHarmonicsGroup_;
	CR42YToggleSelector* wfHarmonicsSelector_;

	CR42YRelativeContainer* wfSamplesGroup_;
	CR42YToggle* wfSamplesEditTgl_;

	bool ignoreTextUpdate_;

	void typeSelectCallback(int selected);
	void posEditCallback(int newPos);
	void functionEditCallback();
	void harmSelectCallback(int selected);
	void sampleEditCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWFPARTEDITOR_H_ */
