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

#include <map>
#include <cstring>

#include "WavetableEditData.h"
#include "WPHarmonics.h"
#include "WPFunction.h"
#include "WPSamples.h"

namespace cr42y
{

WavetableEditData::WavetableEditData(int w) :
		width(w)
{
	addWaveform();
}

WavetableEditData::WavetableEditData(char* data)
{
	int size = *(int*) data;
	data += sizeof(int);
	width = *(int*) data;
	data += sizeof(int);
	size -= sizeof(int);
	char* start = data;
	char** dataPtr = &data;

	while (size > data - start)
	{
		int amount = *(int*) data;
		data += sizeof(int);
		parts.push_back(std::vector<WaveformPart*>());
		for (int i = 0; i < amount; i++)
		{
			WaveformPart* part = WaveformPart::getFromData(dataPtr);
			parts[parts.size() - 1].push_back(part);
		}
	}
}

WavetableEditData::~WavetableEditData()
{
	for (size_t i = 0; i < parts.size(); i++)
	{
		for (size_t j = 0; j < parts[i].size(); j++)
		{
			delete parts[i][j];
		}
		parts[i].clear();
	}
	parts.clear();
}

WavetableEditData& WavetableEditData::operator =(const WavetableEditData& data)
{
	for (size_t i = 0; i < parts.size(); i++)
	{
		for (size_t j = 0; j < parts[i].size(); j++)
		{
			delete parts[i][j];
		}
		parts[i].clear();
	}
	parts.clear();

	for (size_t i = 0; i < data.parts.size(); i++)
	{
		parts.push_back(std::vector<WaveformPart*>());
		for (size_t j = 0; j < data.parts[i].size(); j++)
		{
			WaveformPart* copy = WaveformPart::copy(*(data.parts[i][j]));
			parts[i].push_back(copy);
		}
	}
	return *this;
}

int WavetableEditData::getWidth()
{
	return width;
}

WaveformPart* WavetableEditData::getPartByIndex(int row, int idx)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		if (0 <= idx && (size_t) idx < wf->size())
		{
			return (*wf)[idx];
		}
	}
	return nullptr;
}

int WavetableEditData::getIndexOfPart(int row, WaveformPart* part)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		for (size_t i = 0; i < wf->size(); i++)
		{
			if ((*wf)[i] == part)
			{
				return i;
			}
		}
	}
	return -1;
}

std::vector<std::pair<float, float>> WavetableEditData::getVisibleAreas(int row,
		WaveformPart* part)
{
	std::vector<std::pair<float, float>> ret;
	int idx = getIndexOfPart(row, part);
	if (idx != -1)
	{
		ret.push_back(
				std::pair<float, float>(part->getStart(), part->getEnd()));
		std::vector<WaveformPart*>* wf = getWaveform(row);
		for (idx++; (size_t) idx < wf->size(); idx++)
		{
			for (size_t i = 0; i < ret.size(); i++)
			{
				if ((*wf)[idx]->getStart() <= ret[i].first)
				{
					if ((*wf)[idx]->getEnd() > ret[i].first
							&& (*wf)[idx]->getEnd() < ret[i].second)
					{
						ret[i].first = (*wf)[idx]->getEnd();
					}
					else if ((*wf)[idx]->getEnd() >= ret[i].second)
					{
						ret.erase(ret.begin() + i);
						i--;
					}
				}
				else if ((*wf)[idx]->getStart() < ret[i].second)
				{
					if ((*wf)[idx]->getEnd() > ret[i].first
							&& (*wf)[idx]->getEnd() < ret[i].second)
					{
						ret.push_back(
								std::pair<float, float>((*wf)[idx]->getEnd(),
										ret[i].second));
						ret[i].second = (*wf)[idx]->getStart();
					}
					else if ((*wf)[idx]->getEnd() >= ret[i].second)
					{
						ret[i].second = (*wf)[idx]->getStart();
					}
				}
			}
		}
	}
	return ret;
}

WaveformPart* WavetableEditData::getVisiblePartAtPos(int row, float pos)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	WaveformPart* part = nullptr;
	if (wf)
	{
		for (size_t i = 0; i < wf->size(); i++)
		{
			if ((*wf)[i]->getStart() <= pos && (*wf)[i]->getEnd() >= pos)
			{
				part = (*wf)[i];
			}
		}
	}
	return part;
}

void WavetableEditData::addWaveform(int idx)
{
	std::vector<WaveformPart*> row;
	std::vector<std::pair<float, float>> ht;
	/*ht.push_back(std::pair<float, float>(0, 0));
	 ht.push_back(std::pair<float, float>(1, 0));
	 WPHarmonics* harm = new WPHarmonics(0, 1, ht, WPHarmonics::SIN);
	 row.push_back(harm); //new WPFunction(0, 1, "sin(2pi*x)"));
	 row[0]->setEnd(1);*/
	if (idx < 0 || (size_t) idx >= parts.size())
	{
		parts.push_back(row);
	}
	else
	{
		parts.insert(parts.begin() + idx, row);
	}
}

void WavetableEditData::removeWaveform(int idx, bool erase)
{
	if (idx >= 0 && (size_t) idx < parts.size())
	{
		if (erase)
		{
			for (size_t i = 0; i < parts[idx].size(); i++)
			{
				delete parts[idx][i];
			}
		}
		parts.erase(parts.begin() + idx);
	}
}

void WavetableEditData::addPart(int row, WaveformPart* part, int idx)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		if (idx >= 0 && (size_t) idx < wf->size())
		{
			wf->insert(wf->begin() + idx, part);
		}
		else
		{
			wf->push_back(part);
		}
	}
}

void WavetableEditData::removePart(int row, WaveformPart* part)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		for (size_t i = 0; i < wf->size(); i++)
		{
			if ((*wf)[i] == part)
			{
				delete (*wf)[i];
				wf->erase(wf->begin() + i);
				i--;
			}
		}
	}
}

void WavetableEditData::removePart(int row, int idx, bool erase)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		if (0 <= idx && (size_t) idx < wf->size())
		{
			if (erase)
			{
				delete (*wf)[idx];
			}
			wf->erase(wf->begin() + idx);
		}
	}
}

std::vector<std::vector<WaveformPart*>>* WavetableEditData::getWaveforms()
{
	return &parts;
}

std::vector<WaveformPart*>* WavetableEditData::getWaveform(int row)
{
	if (0 <= row && (size_t) row < parts.size())
	{
		return &parts[row];
	}
	return nullptr;
}

float WavetableEditData::getSample(int row, int smpl)
{
	float ret = 0;
	WaveformPart* part = getVisiblePartAtPos(row, (float) smpl / getWidth());
	if (part)
	{
		ret = part->getSample(width, smpl, row);
	}
	return ret;
}

void WavetableEditData::getSamples(std::vector<float>& samples, int row, int stepSize)
{
	samples.reserve(getWidth() / stepSize);
	for (int i = 0; i < getWidth(); i += stepSize)
	{
		samples.push_back(getSample(row, i));
	}
}

void WavetableEditData::getSamples(std::vector<std::vector<float>>& samples)
{
	samples.resize(parts.size());
	for (size_t i = 0; i < parts.size(); i++)
	{
		getSamples(samples[i], i);
	}
}

void WavetableEditData::getPartSamples(std::vector<float>& samples, int row, int part,
		int stepSize)
{
	WaveformPart* p = getPartByIndex(row, part);
	if (p)
	{
		int start = p->getStart() * getWidth();
		int end = p->getEnd() * getWidth();
		samples.reserve((end - start) / stepSize);
		for (int i = start; i < end; i += stepSize)
		{
			samples.push_back(p->getSample(getWidth(), i, row));
		}
	}
}

int WavetableEditData::getData(void** buffer)
{
	int totalsize = 0;
	std::vector<std::vector<std::pair<void*, int>>> partdata;
	for (size_t i = 0; i < parts.size(); i++)
	{
		totalsize += sizeof(int);
		partdata.push_back(std::vector<std::pair<void*, int>>());
		for (size_t j = 0; j < parts[i].size(); j++)
		{
			void* data = nullptr;
			int size = parts[i][j]->getData(&data);
			partdata[i].push_back(std::pair<void*, int>(data, size));
			totalsize += size;
		}
	}
	totalsize += sizeof(int) * 2;
	char* mem = new char[totalsize];
	*buffer = mem;
	*(int*) mem = totalsize - sizeof(int);
	mem += sizeof(int);
	*(int*) mem = width;
	mem += sizeof(int);
	
	for (size_t i = 0; i < partdata.size(); i++)
	{
		*(int*) mem = partdata[i].size();
		mem += sizeof(int);
		for (size_t j = 0; j < partdata[i].size(); j++)
		{
			memcpy(mem, partdata[i][j].first, partdata[i][j].second);
			mem += partdata[i][j].second;
			delete[] (char*) partdata[i][j].first;
		}
	}
	return totalsize;
}

void WavetableEditData::update(char* data)
{
	int size = *(int*) data;
	data += sizeof(int);
	size_t row = *(int*) data;
	data += sizeof(int);
	size -= sizeof(int);

	char* start = data;
	char** dataPtr = &data;

	if (row >= parts.size())
	{
		row = parts.size();
		parts.push_back(std::vector<WaveformPart*>());
	}
	else
	{
		for (size_t i = 0; i < parts[row].size(); i++)
		{
			delete parts[row][i];
			parts[row][i] = nullptr;
		}
		parts[row].clear();
	}

	while (size > data - start)
	{
		WaveformPart* part = WaveformPart::getFromData(dataPtr);
		parts[row].push_back(part);
	}
}

int WavetableEditData::getUpdate(void** buffer, int row)
{
	int totalsize = 0;
	std::vector<std::pair<void*, int>> partdata;
	for (size_t j = 0; j < parts[row].size(); j++)
	{
		void* data = nullptr;
		int size = parts[row][j]->getData(&data);
		partdata.push_back(std::pair<void*, int>(data, size));
		totalsize += size;
	}

	totalsize += sizeof(int) * 2;

	char* mem = new char[totalsize];
	*buffer = mem;

	*(int*) mem = totalsize - sizeof(int);
	mem += sizeof(int);

	*(int*) mem = row;
	mem += sizeof(int);

	for (size_t i = 0; i < partdata.size(); i++)
	{
		memcpy(mem, partdata[i].first, partdata[i].second);
		mem += partdata[i].second;
		delete[] (char*) partdata[i].first;
	}
	return totalsize;
}

std::string WavetableEditData::to_string()
{
	std::string str = "WTED: [";
	str += "width = " + std::to_string(width);
	str += "\n";
	for (size_t i = 0; i < parts.size(); i++)
	{
		str += "[";
		for (size_t j = 0; j < parts[i].size(); j++)
		{
			str += parts[i][j]->to_string() + " ; ";
		}
		str += "]\n";
	}
	str += "]";
	return str;
}

} /* namespace cr42y */
