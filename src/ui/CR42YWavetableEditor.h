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


#ifndef SRC_UI_CR42YWAVETABLEEDITOR_H_
#define SRC_UI_CR42YWAVETABLEEDITOR_H_

#include "CR42YGrid.h"

namespace cr42y
{

class WavetableEditController;
class CR42YWaveformEditor;
class CR42YWaveformEditorControlPanel;
class CR42YHarmonicsView;
class CR42YWavetableView;
class CR42YButton;
class CR42YHarmonicsEditor;
class CR42YToolSelector;
class CR42YWFPartEditor;
class CR42YWFBulkEditor;
class WavetableEditData;

class CR42YWavetableEditor : public CR42YGrid
{
public:
	CR42YWavetableEditor(CR42YUI* ui);
	virtual ~CR42YWavetableEditor();

	void setData(WavetableEditData* data);

	WavetableEditController* getController();

protected:
	void on_realize();

private:
	WavetableEditController* controller_;

	CR42YWaveformEditor* wfEditor_;
	CR42YWaveformEditorControlPanel* wfeControllerPanel_;
	CR42YHarmonicsView* harmonicsView_;
	CR42YWavetableView* wtView_;
	CR42YHarmonicsEditor* harmonicsEditor_;
	CR42YToolSelector* toolSelector_;
	CR42YButton* addWaveformButton_;
	CR42YWFPartEditor* wfPartEditor_;
	CR42YWFBulkEditor* wfBulkEditor_;

	void waveformAddCallback();

	/*void waveformUpdateCallback();
	void wavetableUpdateCallback();

	void partSelectCallback();*/

	void selectionChangedCallback();
	void selectionChangedDoneCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVETABLEEDITOR_H_ */
