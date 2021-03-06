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


#ifndef SRC_UI_CR42YWFBULKEDITOR_H_
#define SRC_UI_CR42YWFBULKEDITOR_H_

#include "CR42YGrid.h"

namespace cr42y
{
class WavetableEditController;
class CR42YToggleSelector;
class CR42YButton;
class CR42YLabel;
class CR42YEntry;
class CR42YIntegerEditor;

class CR42YWFBulkEditor : public CR42YGrid
{
public:
	CR42YWFBulkEditor(CR42YUI* ui);
	virtual ~CR42YWFBulkEditor();

	void setController(WavetableEditController* controller);

	void update();

protected:
	void on_realize();
	void on_show();

private:
	WavetableEditController* controller_;

	CR42YToggleSelector* opSelector_;
	CR42YButton* addBtn_;

	CR42YGrid* mathGroup_;
	CR42YLabel* mathFuncLabel_;
	CR42YEntry* mathFuncEntry_;
	CR42YLabel* mathStartLabel_;
	CR42YIntegerEditor* mathStartEditor_;
	CR42YLabel* mathAmntLabel_;
	CR42YIntegerEditor* mathAmntEditor_;

	CR42YGrid* wavGroup_;
	CR42YEntry* wavFileEntry_;
	CR42YButton* wavFileBtn_;
	CR42YLabel* wavStartLabel_;
	CR42YIntegerEditor* wavStartEditor_;
	CR42YToggleSelector* wavAmntTypeSelector_;
	CR42YIntegerEditor* wavAmntEditor_;

	CR42YGrid* morphGroup_;
	CR42YLabel* morphStartLabel_;
	CR42YIntegerEditor* morphStartEditor_;
	CR42YLabel* morphAmntLabel_;
	CR42YIntegerEditor* morphAmntEditor_;
	CR42YToggleSelector* morphTypeSelector_;

	void opSelectCallback(int selected);
	void addCallback();
	void fileChooseCallback();
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWFBULKEDITOR_H_ */
