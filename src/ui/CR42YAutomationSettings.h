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

#ifndef CR42Y_CR42YAUTOMATIONSETTINGS_H
#define CR42Y_CR42YAUTOMATIONSETTINGS_H

#include "CR42YGrid.h"
#include <sigc++/sigc++.h>

namespace cr42y
{

class AutomationEditController;
class CR42YLabel;
class CR42YIntegerEditor;
class CR42YControlIntegerEditor;
class CR42YControlToggleSelector;
class CR42YControlDial;

class CR42YAutomationSettings : public CR42YGrid
{
public:

	CR42YAutomationSettings(CR42YUI* ui, AutomationEditController* controller);

	virtual ~CR42YAutomationSettings();
	
	void update();
	
	sigc::signal<void, int> signalGridX();
	sigc::signal<void, int> signalGridY();

protected:
	void on_realize();
private:
	AutomationEditController* controller_;
	
	CR42YLabel* gridXLabel_;
	CR42YLabel* gridYLabel_;
	CR42YIntegerEditor* gridXEditor_;
	CR42YIntegerEditor* gridYEditor_;
	
	CR42YGrid* settingsGrid_;
	CR42YControlToggleSelector* typeSelector_;
	
	CR42YLabel* sustainLabel_;
	CR42YControlIntegerEditor* sustainEditor_;
	
	CR42YControlToggleSelector* syncSelector_;
	CR42YControlToggleSelector* timingSelector_;
	
	
	CR42YGrid* secondsTimingGrid_;
	CR42YLabel* secondsLabel_;
	CR42YControlDial* secondsDial_;
	
	CR42YGrid* beatsTimingGrid_;
	CR42YLabel* beatsLabel_;
	CR42YControlIntegerEditor* numeratorEditor_;
	CR42YLabel* lineLabel_;
	CR42YControlIntegerEditor* denominatorEditor_;
	
	void timingSelectCallback(int selected);
	void typeSelectCallback(int selected);
};

}

#endif // CR42Y_CR42YAUTOMATIONSETTINGS_H
