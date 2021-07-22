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

#ifndef CR42Y_CR42YMODULATIONEDITOR_H
#define CR42Y_CR42YMODULATIONEDITOR_H

#include "CR42YSquareContainer.h"

namespace cr42y
{
class CR42YControlDial;
class CR42YGrid;
class CR42YnthCommunicator;
class CR42YToggleSelector;
class ModulationControls;

class CR42YModulationEditor : public CR42YSquareContainer
{
public:
	CR42YModulationEditor(CR42YUI* ui, CR42YnthCommunicator* communicator);

    virtual ~CR42YModulationEditor();

private:
	CR42YGrid* grid_;
	CR42YToggleSelector* modSelector_;
	CR42YSquareContainer* modSquare_;
	CR42YGrid* modGrid_;
	
	std::vector<CR42YControlDial*> dials_;
	
	ModulationControls* controls_;
	
	void connectControls();
	
	void selectCallback(int selected);
};

}

#endif // CR42Y_CR42YMODULATIONEDITOR_H
