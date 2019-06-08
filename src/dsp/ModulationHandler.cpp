/*******************************************************************************
 * Copyright (c) 2019 krokoschlange and contributors.
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

#include "ModulationHandler.h"
#include "CR42Ynth.h"

namespace cr42y
{

ModulationHandler::ModulationHandler(Voice* v) :
		voice(v)
{
}

ModulationHandler::~ModulationHandler()
{
}

void ModulationHandler::calculateModulation(int oscIndex,
		OscillatorController** controllers)
{
	if (oscIndex > 0)
	{
		float fmMult = 2;
		std::vector<std::vector<ModulatableDoubleControl>>* fmControls =
				CR42Ynth::getInstance()->getFMControls();
		std::vector<std::vector<ModulatableDoubleControl>>* pmControls =
				CR42Ynth::getInstance()->getPMControls();
		std::vector<std::vector<ModulatableDoubleControl>>* amControls =
				CR42Ynth::getInstance()->getAMControls();
		std::vector<std::vector<ModulatableDoubleControl>>* rmControls =
				CR42Ynth::getInstance()->getRMControls();
		float fm = 0;
		float pm = 0;
		float am = 0;
		float rm = 0;
		for (int i = 0; i < (*fmControls)[oscIndex - 1].size(); i++)
		{
			fm +=
					controllers[i]->getValue() * fmMult * (*fmControls)[oscIndex - 1][i].getValue();
		}
		for (int i = 0; i < (*pmControls)[oscIndex - 1].size(); i++)
		{
			pm +=
					controllers[i]->getValue() * (*pmControls)[oscIndex - 1][i].getValue();
		}
		for (int i = 0; i < (*amControls)[oscIndex - 1].size(); i++)
		{
			am *=
					(controllers[i]->getValue() * ((*amControls)[oscIndex - 1][i].getValue() / 2)) + 1 - ((*amControls)[oscIndex - 1][i].getValue() / 2);
		}
		for (int i = 0; i < (*rmControls)[oscIndex - 1].size(); i++)
		{
			rm *=
					abs(controllers[i]->getValue()) * (*rmControls)[oscIndex - 1][i].getValue() + 1 - (*rmControls)[oscIndex - 1][i].getValue();
		}

		controllers[oscIndex]->setFM(fm);
		controllers[oscIndex]->setFM(pm);
		controllers[oscIndex]->setFM(am);
		controllers[oscIndex]->setFM(rm);
	}
}

} /* namespace cr42y */
