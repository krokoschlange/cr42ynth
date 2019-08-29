/*
 * WavetableEditData.cpp
 *
 *  Created on: 07.08.2019
 *      Author: fabian
 */

#include <map>
#include <cstring>

#include "WavetableEditData.h"

namespace cr42y
{

WavetableEditData::WavetableEditData(int w) :
		width(w)
{
	addWaveform(-1);
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
		WaveformPart* part = new WaveformPart(dataPtr);
		if (part->getStart() == 0 && part->getBase())
		{
			parts.push_back(std::vector<WaveformPart*>());
		}
		parts[parts.size() - 1].push_back(part);
	}
}

WavetableEditData::~WavetableEditData()
{
	for (int i = 0; i < parts.size(); i++)
	{
		for (int j = 0; j < parts[i].size(); j++)
		{
			delete parts[i][j];
		}
		parts[i].clear();
	}
	parts.clear();
}

int WavetableEditData::getWidth()
{
	return width;
}

void WavetableEditData::addWaveform(int idx)
{
	std::string* func = new std::string("sin(2pi*x)");
	std::vector<WaveformPart*> row;
	row.push_back(new WaveformPart(0, FUNCTION, true, func));
	if (idx == -1 || idx >= parts.size())
	{
		parts.push_back(row);
	}
	else
	{
		parts.insert(parts.begin() + idx, row);
	}
}

void WavetableEditData::setBase(int row, WaveformPart* part)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		for (int i = 0; i < wf->size(); i++)
		{
			if ((*wf)[i]->getBase())
			{
				if (!((*wf)[i]->getFunction() && !(*wf)[i]->getFunction()->compare("base()")))
				{
					removePart(row, (*wf)[i]);
					addPart(row, part, 0);
					return;
				}
			}
		}
		addPart(row, part, 0);
	}
}

void WavetableEditData::addPart(int row, WaveformPart* part, float end)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		int idx = 0;
		for (; idx < wf->size() && (*wf)[idx]->getStart() < part->getStart();
				idx++)
		{
		}
		wf->insert(wf->begin() + idx, part);

		float currentEnd = 1;
		if (idx < wf->size() - 1)
		{
			currentEnd = (*wf)[idx + 1]->getStart();
		}
		if (currentEnd > end)
		{
			if (idx > 0)
			{
				WaveformPart* p = new WaveformPart((*wf)[idx - 1], end, getWidth());
				wf->insert(wf->begin() + idx + 1, p);
			}
			else
			{
				if (wf->size() > 1)
				{
					(*wf)[idx + 1]->setStart(end);
				}
			}
		}
		else
		{
			for (idx++; idx < wf->size(); idx++)
			{
				currentEnd = 1;
				if (idx < wf->size() - 1)
				{
					currentEnd = (*wf)[idx + 1]->getStart();
				}
				if (currentEnd < end)
				{
					delete (*wf)[idx];
					wf->erase(wf->begin() + idx);
					idx--;
				}
				else
				{
					(*wf)[idx]->setStart(end);
				}
			}
		}
	}
}

void WavetableEditData::removePart(int row, WaveformPart* part)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		for (int i = 0; i < wf->size(); i++)
		{
			if ((*wf)[i] == part)
			{
				wf->erase(wf->begin() + i);
				i--;
			}
		}

		for (int i = 1; i < wf->size(); i++)
		{
			if ((*wf)[i]->getBase())
			{
				if ((*wf)[i - 1]->getBase())
				{
					if ((*wf)[i]->getFunction() && !(*wf)[i]->getFunction()->compare("base()"))
					{
						delete (*wf)[i];
						wf->erase(wf->begin() + i);
						i--;
					}
					else
					{
						delete (*wf)[i - 1];
						wf->erase(wf->begin() + i - 1);
						i--;
					}
				}
			}
			else
			{
				if ((*wf)[i]->getType() == (*wf)[i - 1]->getType())
				{
					switch ((*wf)[i]->getType())
					{
					case FUNCTION:
						if (!(*wf)[i]->getFunction()->compare(*(*wf)[i - 1]->getFunction()))
						{
							delete (*wf)[i];
							wf->erase(wf->begin() + i);
							i--;
						}
						break;
					case SAMPLES:
						for (int j = 0; j < (*wf)[i]->getSamples()->size(); j++)
						{
							(*wf)[i - 1]->getSamples()->push_back((*(*wf)[i]->getSamples())[j]);
						}
						delete (*wf)[i];
						wf->erase(wf->begin() + i);
						i--;
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

std::vector<WaveformPart*>* WavetableEditData::getWaveform(int row)
{
	if (0 <= row && row < parts.size())
	{
		return &parts[row];
	}
	return nullptr;
}

WaveformPart* WavetableEditData::getBase(int row)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	if (wf)
	{
		for (int i = 0; i < wf->size(); i++)
		{
			if ((*wf)[i]->getBase() && !((*wf)[i]->getFunction() && !(*wf)[i]->getFunction()->compare("base()")))
			{
				return (*wf)[i];
			}
		}
	}
	return nullptr;
}

float WavetableEditData::getSample(int row, int smpl)
{
	std::vector<WaveformPart*>* wf = getWaveform(row);
	float ret = 0;
	if (wf)
	{
		float pos = (float) smpl / getWidth();
		WaveformPart* part = nullptr;
		for (int i = 0; i < wf->size(); i++)
		{
			if (!part || ((*wf)[i]->getStart() > part->getStart() && (*wf)[i]->getStart() < pos))
			{
				part = (*wf)[i];
			}
		}
		if (part)
		{
			ret = part->getSample(width, smpl);
		}
	}
	return ret;
}

std::vector<float>* WavetableEditData::getSamples(int row)
{
	std::vector<float>* ret = new std::vector<float>();
	for (int i = 0; i < getWidth(); i++)
	{
		ret->push_back(getSample(row, i));
	}
	return ret;
}

std::vector<std::vector<float>>* WavetableEditData::getSamples()
{
	std::vector<std::vector<float>>* ret = new std::vector<std::vector<float>>();
	for (int i = 0; i < parts.size(); i++)
	{
		ret->push_back(*getSamples(i));
	}
	return ret;
}

int WavetableEditData::getData(void** buffer)
{
	int totalsize = 0;
	std::map<void*, int> partdata;
	for (int i = 0; i < parts.size(); i++)
	{
		for (int j = 0; j < parts[i].size(); j++)
		{
			void* data = nullptr;
			int size = parts[i][j]->getData(&data);
			partdata.insert(std::pair<void*, int>(data, size));
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
	for (std::map<void*, int>::iterator it = partdata.begin();
			it != partdata.end(); it++)
	{
		memcpy(mem, it->first, it->second);
		mem += it->second;
		delete[] (char*) it->first;
	}
	return totalsize;
}

void WavetableEditData::update(char* data)
{
	int size = *(int*) data;
	data += sizeof(int);
	int row = *(int*) data;
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
		for (int i = 0; i < parts[row].size(); i++)
		{
			delete parts[row][i];
			parts[row][i] = nullptr;
		}
		parts[row].clear();
	}

	while (size > data - start)
	{
		WaveformPart* part = new WaveformPart(dataPtr);
		parts[row].push_back(part);
	}
}

int WavetableEditData::getUpdate(void** buffer, int row)
{
	int totalsize = 0;
	std::vector<std::pair<void*, int>> partdata;
	for (int j = 0; j < parts[row].size(); j++)
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

	for (int i = 0; i < partdata.size(); i++)
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
	for (int i = 0; i < parts.size(); i++)
	{
		str += "[";
		for (int j = 0; j < parts[i].size(); j++)
		{
			str += parts[i][j]->to_string() + ", ";
		}
		str += "]\n";
	}
	str += "]";
	return str;
}

} /* namespace cr42y */
