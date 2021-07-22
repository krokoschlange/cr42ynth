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

#ifndef CR42Y_AUTOMATIONEDITCONTROLLER_H
#define CR42Y_AUTOMATIONEDITCONTROLLER_H

#include <vector>
#include <sigc++/sigc++.h>

namespace cr42y
{
class AutomationData;
class Control;
class CR42YnthCommunicator;
	
class AutomationEditController
{
public:
	AutomationEditController(CR42YnthCommunicator* communicator);

	virtual ~AutomationEditController();
	
	void samples(std::vector<float>& samples, size_t amount);
	size_t sectionAmount();
	float handle(size_t section);
	float handlePosition(size_t section);
	void sectionHandle(float* x, float* y, size_t section);
	
	void moveSection(size_t section, float x);
	void moveSectionHandle(size_t section, float y);
	void moveHandle(size_t section, float y);
	
	size_t createSection(float x, float y);
	void removeSection(size_t section);
	
	float getSectionHeight(size_t section);
	
	size_t createAutomation();
	void removeAutomation(size_t id);
	
	void selectAutomation(size_t automation);
	size_t selectedAutomation();
	
	size_t getAutomationCount();
	int getType(size_t automation);
	size_t getID(size_t automation);
	
	Control* useBeatsLengthControl();
	Control* lengthSecondsControl();
	Control* lengthBeatsNumeratorControl();
	Control* lengthBeatsDenominatorControl();
	Control* typeControl();
	Control* syncControl();
	Control* sustainControl();
	
	sigc::signal<void, size_t>& selectionChangedSignal();
	sigc::signal<void>& dataChangedSignal();

private:
	struct Automation
	{
		uint32_t id_;
		AutomationData* data_;
		Control* useBeatsLength_;
		Control* lengthSeconds_;
		Control* lengthBeatsNumerator_;
		Control* lengthBeatsDenominator_;
		Control* typeControl_;
		Control* syncControl_;
		Control* sustainControl_;
	};
	
	CR42YnthCommunicator* communicator_;
	std::vector<Automation> automations_;
	size_t selected_;
	
	sigc::signal<void, size_t> selectionChangedSignal_;
	sigc::signal<void> dataChangedSignal_;
	
	void deleteAutomation(Automation& data);
};

}

#endif // CR42Y_AUTOMATIONEDITCONTROLLER_H
