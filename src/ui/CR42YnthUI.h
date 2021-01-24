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


#ifndef SRC_UI_CR42YNTHUI_H_
#define SRC_UI_CR42YNTHUI_H_

#include "CR42YRelativeContainer.h"
#include "CR42YUI.h"

namespace cr42y
{

class CR42YnthCommunicator;
class OSCEvent;
class CR42YToggleSelector;
class CR42YOSCSettings;
class CR42YWavetableEditor;
class CR42YModulationEditor;

class CR42YnthUI : public CR42YUI
{
public:
	CR42YnthUI(CR42YnthCommunicator* comm, const char* path);
	virtual ~CR42YnthUI();
	
	const char* getBundlePath();

	void idle();

	CR42YnthCommunicator* getCommunicator();

protected:
	void on_realize();
	void on_show();

private:
	Gtk::Main* gtkMain_;

	char* bundlePath_;

	CR42YnthCommunicator* communicator_;

	CR42YToggleSelector* screenSelector_;
	int selectedScreen_;
	
	CR42YOSCSettings* oscSettings_;
	CR42YWavetableEditor* wtEditor_;
	CR42YModulationEditor* modEditor_;


	void screenSelectCallback(int selected);

};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YNTHUI_H_ */
