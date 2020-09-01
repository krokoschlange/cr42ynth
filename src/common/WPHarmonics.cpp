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

#include <cmath>
#include <cstring>
#include <iostream>

#include "WPHarmonics.h"

namespace cr42y
{

WPHarmonics::WPHarmonics(float s, float e, std::vector<std::pair<float, float>> ht, functionType t) :
		WaveformPart(s, e, WaveformPart::HARMONICS),
		hTable(ht),
		fType(t),
		normFactor(1),
		needUpdate(true)
{
	for (int i = hTable.size(); i < 129; i++)
	{
		hTable.push_back(std::pair<float, float>(0, 0));
	}
	//normalize();
}

WPHarmonics::WPHarmonics(float s, float e, char** data, int size) :
		WaveformPart(s, e, WaveformPart::HARMONICS),
		normFactor(1),
		needUpdate(true)
{
	fType = *(functionType*) *data;
	*data += sizeof(int);
	size -= sizeof(int);
	for (int i = 0; i < size; i += 2 * sizeof(float))
	{
		float first = *(float*) *data;
		*data += sizeof(float);
		float second = *(float*) *data;
		*data += sizeof(float);
		hTable.push_back(std::pair<float, float>(first, second));
	}
}

WPHarmonics::~WPHarmonics()
{
}

int WPHarmonics::getData(void** buffer)
{
	PartDataHead* head = WaveformPart::getDataHead();
	int size = hTable.size() * 2 * sizeof(float) + sizeof(int);
	head->size = size;
	
	int totalSize = sizeof(PartDataHead) + size;
	
	char* mem = new char[totalSize];
	*buffer = mem;
	
	memcpy(mem, head, sizeof(PartDataHead));
	mem += sizeof(PartDataHead);
	
	*(int*) mem = fType;
	mem += sizeof(int);
	
	for (int i = 0; i < hTable.size(); i++)
	{
		*(float*) mem = hTable[i].first;
		mem += sizeof(float);
		*(float*) mem = hTable[i].second;
		mem += sizeof(float);
	}
	
	delete head;
	
	return totalSize;
}

float WPHarmonics::getSample(int size, int pos)
{
	if (samples.size() != size || needUpdate)
	{
		update(size);
	}
	return samples[pos];
}

void WPHarmonics::update(int size)
{
	needUpdate = false;
	samples.clear();
	for (int pos = 0; pos < size; pos++)
	{
		float x = (float) pos / size;
		float smpl = 0;
		switch (fType)
		{
		case SIN:
			for (int i = 0; i < hTable.size(); i++)
			{
				if (hTable[i].first != 0)
				{
					smpl += hTable[i].first * sinf(2 * M_PI * (i * x - hTable[i].second));
				}
			}
			break;
		case TRI:
			break;
		case SAW:
			for (int i = 0; i < hTable.size(); i++)
			{
				float sawX = (i * x - hTable[i].second);
				if (sawX < 0)
				{
					sawX = 1 + (sawX - (int) sawX);
				}
				else if (sawX > 1)
				{
					sawX = sawX - (int) sawX;
				}
				smpl += hTable[i].first * (2 * sawX - 1);
			}
			break;
		case SQR:
			for (int i = 0; i < hTable.size(); i++)
			{
				float sqrX = (i * x - hTable[i].second);
				if (sqrX < 0)
				{
					sqrX = 1 + (sqrX - (int) sqrX);
				}
				else if (sqrX > 1)
				{
					sqrX = sqrX - (int) sqrX;
				}
				smpl += hTable[i].first * (2 * (sqrX > 0.5) - 1);
			}
			break;
		default:
			break;
		}
		samples.push_back(smpl);
	}
}

void WPHarmonics::setUpdate()
{
	needUpdate = true;
}

std::string WPHarmonics::to_string()
{
	std::string str = WaveformPart::to_string();
	str += " (HARMONICS): " + std::to_string(fType);
	return str;
}

void WPHarmonics::setFunctionType(functionType t)
{
	if (t != fType)
	{
		fType = t;
		setUpdate();
	}
}

int WPHarmonics::getFunctionType()
{
	return fType;
}

std::vector<std::pair<float, float>>* WPHarmonics::getHarmonicTable()
{
	return &hTable;
}

void WPHarmonics::normalize()
{
	normFactor = 1;
	float maxAmp = 0;
	for (int i = 0; i < 256; i++)
	{
		float smpl = getSample(256, i);
		if (fabsf(smpl) > maxAmp)
		{
			maxAmp = fabsf(smpl);
		}
	}
	normFactor = maxAmp;
	for (int i = 0; i < hTable.size(); i++)
	{
		hTable[i].first /= normFactor;
	}
	needUpdate = true;
}

} /* namespace cr42y */

