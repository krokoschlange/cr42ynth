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

#include <iostream>
#include <cstring>

#include "WPSamples.h"

namespace cr42y
{

WPSamples::WPSamples(float s, float e, std::vector<float> sam) :
		WaveformPart(s, e, WaveformPart::SAMPLES),
		samples(sam)
{
}

WPSamples::WPSamples(float s, float e, char** data, int size) :
		WaveformPart(s, e, WaveformPart::SAMPLES)
{
	for (int i = 0; i < size; i += sizeof(float))
	{
		samples.push_back(*(float*) *data);
		*data += sizeof(float);
	}
}

WPSamples::~WPSamples()
{
}

int WPSamples::getData(void** buffer)
{
	PartDataHead* head = WaveformPart::getDataHead();
	int size = samples.size() * sizeof(float);
	head->size = size;
	
	int totalSize = sizeof(PartDataHead) + size;
	
	char* mem = new char[totalSize];
	*buffer = mem;
	
	memcpy(mem, head, sizeof(PartDataHead));
	mem += sizeof(PartDataHead);
	
	for (int i = 0; i < samples.size(); *(float*) mem = samples[i], mem += sizeof(float), i++) {}
	
	delete head;
	
	return totalSize;
}

float WPSamples::getSample(int size, int pos, int ypos)
{
	int startPos = size * getStart();
	int relPos = pos - startPos;
	if (relPos >= 0 && relPos < samples.size())
	{
		return samples[relPos];
	}
	return 0;
}

std::string WPSamples::to_string()
{
	std::string str = WaveformPart::to_string();
	str += " (SAMPLES)";
	return str;
}

std::vector<float>* WPSamples::getSamples()
{
	return &samples;
}

} /* namespace cr42y */
