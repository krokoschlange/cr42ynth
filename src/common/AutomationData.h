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

#ifndef CR42Y_AUTOMATIONDATA_H
#define CR42Y_AUTOMATIONDATA_H

#include <vector>
#include <bits/stdint-uintn.h>

namespace cr42y
{

class AutomationData
{
public:
	AutomationData();

	AutomationData(const AutomationData& other);

	AutomationData(uint8_t* data);

	virtual ~AutomationData();

	AutomationData& operator=(const AutomationData& other);
	
	size_t getData(void** buffer);

	void moveSection(size_t section, float newStart);
	void moveStartValue(size_t section, float value);
	void moveHandle(size_t section, float value);

	void getSamples(std::vector<float>& samples, size_t length);

	struct Section
	{
		float start;
		float startValue;
		float handle;
	};

	size_t addSection(Section section);
	void removeSection(size_t section);
	
	std::vector<Section>& getSections();
	float getSectionSample(size_t section, float position);

private:
	std::vector<Section> sections_;


	float handleSigmoid(float handle);
};

}

#endif // CR42Y_AUTOMATIONDATA_H
