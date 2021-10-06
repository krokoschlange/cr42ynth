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

#ifndef CR42Y_CR42YAUTOMATIONEDITOR_H
#define CR42Y_CR42YAUTOMATIONEDITOR_H

#include "CR42YGrid.h"

namespace cr42y
{
class AutomationEditController;
class CR42YnthCommunicator;
class CR42YAutomationSampleEditor;
class CR42YAutomationSelector;
class CR42YIntegerEditor;
class CR42YButton;
class CR42YAutomationSettings;

class CR42YAutomationEditor : public CR42YGrid
{
public:
	CR42YAutomationEditor(CR42YUI* ui, CR42YnthCommunicator* communicator, AutomationEditController* automationEditController);

	virtual ~CR42YAutomationEditor();

private:
	AutomationEditController* controller_;
	CR42YnthCommunicator* communicator_;
	
	CR42YAutomationSampleEditor* sampleEditor_;
	//CR42YIntegerEditor* gridXEditor_;
	//CR42YIntegerEditor* gridYEditor_;
	
	CR42YAutomationSelector* selector_;
	
	CR42YButton* addBtn_;
	CR42YButton* removeBtn_;
	
	CR42YAutomationSettings* settings_;
	
	void gridXChange(int newGrid);
	void gridYChange(int newGrid);
	void selectionChange(size_t selected);
	void selected(int selection);
	void dataChange();
	void addCallback();
	void removeCallback();
};

}

#endif // CR42Y_CR42YAUTOMATIONEDITOR_H
