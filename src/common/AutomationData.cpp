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

#include "AutomationData.h"

#include <cmath>

namespace cr42y
{

AutomationData::AutomationData()
{
	sections_.push_back({0, 0.5, 0});
	sections_.push_back({0.5, 0.5, 0});
	sections_.push_back({1, 0, 1});
}

AutomationData::AutomationData(const AutomationData& other)
{
	sections_ = other.sections_;
}

AutomationData::AutomationData(uint8_t* data)
{
	uint8_t* itptr = data;
	size_t sectionCount = *((size_t*) itptr);
	itptr += sizeof(size_t);
	
	for (size_t i = 0; i < sectionCount; i++)
	{
		sections_.push_back(*((Section*) itptr));
		itptr += sizeof(Section);
	}
}


AutomationData::~AutomationData()
{
	
}

AutomationData& AutomationData::operator=(const AutomationData& other)
{
	sections_ = other.sections_;
	return *this;
}

size_t AutomationData::getData(void ** buffer)
{
	size_t size = sizeof(Section) * sections_.size() + sizeof(size_t);
	uint8_t* mem = new uint8_t[size];
	*buffer = mem;
	
	*((size_t*) mem) = sections_.size();
	mem += sizeof(size_t);
	
	for (size_t i = 0; i < sections_.size(); i++)
	{
		*((Section*) mem) = sections_[i];
		mem += sizeof(Section);
	}
	return size;
}

void AutomationData::moveSection(size_t section, float newStart)
{
	if (section > 0 && section < sections_.size() - 1 && newStart >= 0 && newStart <= 1)
	{
		Section sec = sections_[section];
		sec.start = newStart;
		sections_.erase(sections_.begin() + section);
		size_t newIndex = 1;
		while (newIndex < sections_.size() - 1 && sections_[newIndex].start <= newStart)
		{
			newIndex++;
		}
		if (newIndex > sections_.size() - 1)
		{
			newIndex = sections_.size() - 1;
		}
		sections_.insert(sections_.begin() + newIndex, sec);
	}
}

void AutomationData::moveStartValue(size_t section, float value)
{
	if (section < sections_.size() && value >= 0 && value <=1)
	{
		sections_[section].startValue = value;
	}
}

void AutomationData::moveHandle(size_t section, float value)
{
	if (section < sections_.size())
	{
		sections_[section].handle = value;
	}
}

void AutomationData::getSamples(std::vector<float>& samples, size_t length)
{
	samples.clear();
	samples.reserve(length);
	size_t sample = 0;
	for (size_t i = 0; i < sections_.size() - 1; i++)
	{
		float relativePos = (float) sample / length;
		float relSectionPos = (relativePos - sections_[i].start) / (sections_[i + 1].start - sections_[i].start);
		while (relativePos < sections_[i + 1].start)
		{
			samples.push_back(getSectionSample(i, relSectionPos));
			sample++;
			relativePos = (float) sample / length;
			relSectionPos = (relativePos - sections_[i].start) / (sections_[i + 1].start - sections_[i].start);
		}
	}
}

size_t AutomationData::addSection(cr42y::AutomationData::Section section)
{
	size_t index = 1;
	while (index < sections_.size() - 1 && sections_[index].start <= section.start)
	{
		index++;
	}
	sections_.insert(sections_.begin() + index, section);
	return index;
}

void AutomationData::removeSection(size_t section)
{
	if (section < sections_.size() && sections_.size() > 2)
	{
		sections_.erase(sections_.begin() + section);
		if (sections_.size() > 0)
		{
			sections_[sections_.size() - 1].start = 1;
			sections_[0].start = 0;
		}
	}
}

float AutomationData::getSectionSample(size_t section, float position)
{
	if (section >= sections_.size() || position < 0 || position > 1)
	{
		return 0;
	}
	else if (section >= sections_.size() - 1)
	{
		return sections_[section].startValue;
	}
	float startVal = sections_[section].startValue;
	float endVal = sections_[section + 1].startValue;
	bool isFalling = startVal < endVal;
	
	float relativeHandle = handleSigmoid(sections_[section].handle);
	
	float funcParam = isFalling ? (1 / relativeHandle) - 1 : (1 / (-relativeHandle + 1)) - 1;
	float sample = 0;
	if (funcParam < 1e-20)
	{
		sample = 1;
	}
	else if (std::isinf(funcParam))
	{
		sample = 0;
	}
	else if (std::abs(funcParam - 1) < 0.000001)
	{
		sample = 1 - position;
	}
	else
	{
		//magic
		sample = ((1 / (position * (funcParam - 1) + 1)) - (1 / funcParam)) * (1 / (1 - (1 / funcParam)));
	}
	
	sample = isFalling ? (1 - sample) : sample;
	sample *= std::abs((startVal - endVal));
	sample += isFalling ? startVal : endVal;
	return sample;
}


float AutomationData::handleSigmoid(float handle)
{
	//more magic
	handle *= 4;
	return 1 / (1 + std::exp(-handle - (1 / 12) * std::pow(handle, 3)));
}

std::vector<AutomationData::Section>& AutomationData::getSections()
{
	return sections_;
}


}

