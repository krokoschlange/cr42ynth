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


#ifndef SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_
#define SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class CR42YButton;
class CR42YLabel;
class CR42YIntegerEditor;
class CR42YWaveformEditor;
class WavetableEditController;

class CR42YWaveformEditorControlPanel : public CR42YRelativeContainer
{
public:
	CR42YWaveformEditorControlPanel(CR42YUI* ui, CR42YWaveformEditor* wfEditor, WavetableEditController* controller);
	virtual ~CR42YWaveformEditorControlPanel();

	void gridXCallback(int val);
	void gridYCallback(int val);
	void toSinCallback(bool highQuality);
	void normalizeCallback();
	void nextCallback();
	void prevCallback();
	void deleteCallback();

private:
	CR42YWaveformEditor* wfEditor_;
	WavetableEditController* controller_;

	CR42YLabel* gridXLabel_;
	CR42YIntegerEditor* gridXEditor_;
	CR42YLabel* gridYLabel_;
	CR42YIntegerEditor* gridYEditor_;
	CR42YButton* toSinBtn_;
	CR42YButton* toSinHQBtn_;
	CR42YButton* normalizeBtn_;

	CR42YButton* nextBtn_;
	CR42YButton* prevBtn_;
	CR42YButton* deleteBtn_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVEFORMEDITORCONTROLPANEL_H_ */
