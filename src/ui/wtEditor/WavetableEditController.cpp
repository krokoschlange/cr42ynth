/*
 * WavetableEditController.cpp
 *
 *  Created on: 22.05.2020
 *      Author: fabian
 */

#include "WavetableEditController.h"
#include "WavetableEditData.h"
#include "WPHarmonics.h"
#include <cmath>

#include "FreeTool.h"
#include "SinHalfTool.h"
#include "SinSlopeTool.h"
#include "TriTool.h"

#include "FftRealPair.hpp"

namespace cr42y
{

WavetableEditController::WavetableEditController() :
		data(nullptr),
		wtPos(0),
		selectedParts(),
		tool(TRI_SLOPE),
		dirty(false),
		usedTool(nullptr),
		gridX(0),
		gridY(0)
{
	
}

WavetableEditController::~WavetableEditController()
{
}

void WavetableEditController::setData(WavetableEditData* dt)
{
	if (data != dt)
	{
		data = dt;
		if (data)
		{
			selectedParts.clear();
			for (int i = 0; i < data->getWaveforms()->size(); i++)
			{
				selectedParts.push_back(0);
			}
		}
		else
		{
			selectedParts.clear();
			selectedParts.push_back(0);
		}
		markDirty();
	}
}

WavetableEditData* WavetableEditController::getData()
{
	return data;
}

int WavetableEditController::getWaveformWidth()
{
	if (data)
	{
		return data->getWidth();
	}
	return 2;
}

int WavetableEditController::getWavetableHeight()
{
	if (data)
	{
		return data->getWaveforms()->size();
	}
	return 0;
}

std::vector<float>* WavetableEditController::getSamples(int row, int stepSize)
{
	if (!data)
	{
		std::vector<float>* ret = new std::vector<float>;
		ret->push_back(0);
		ret->push_back(0);
		return ret;
	}
	return data->getSamples(row, stepSize);
}

void WavetableEditController::selectWaveform(int num)
{
	num = num >= selectedParts.size() ? selectedParts.size() - 1 : num;
	num = num < 0 ? 0 : num;
	wtPos = num;
}

int WavetableEditController::getSelectedWaveform()
{
	return wtPos;
}

void WavetableEditController::selectPart(int part)
{
	if (data)
	{
		part = part >= data->getWaveform(wtPos)->size() ?
				data->getWaveform(wtPos)->size() - 1 : part;
		part = part < 0 ? 0 : part;
		selectedParts[wtPos] = part;
	}
}

int WavetableEditController::getSelectedPart()
{
	if (data)
	{
		return selectedParts[wtPos];
	}
	else
	{
		return 0;
	}
}

void WavetableEditController::addWaveform(int idx)
{
	if (data)
	{
		data->addWaveform(idx);
		if (idx < 0 || idx >= selectedParts.size())
		{
			selectedParts.push_back(0);
		}
		else
		{
			selectedParts.insert(selectedParts.begin() + idx, 0);
		}
		markDirty();
	}
}

void WavetableEditController::removeWaveform(int idx)
{
	if (data && idx > 0 && idx < selectedParts.size())
	{
		data->removeWaveform(idx);
		selectedParts.erase(selectedParts.begin() + idx);
		if (wtPos >= data->getWaveforms()->size())
		{
			wtPos = data->getWaveforms()->size() - 1;
		}
		markDirty();
	}
}

bool WavetableEditController::addPart(WaveformPart* part, int idx)
{
	if (data)
	{
		data->addPart(wtPos, part, idx);
		markDirty();
		return true;
	}
	return false;
}

void WavetableEditController::removePart(int idx)
{
	if (data && idx > 0 && idx < data->getWaveform(wtPos)->size())
	{
		data->removePart(wtPos, idx);
		if (selectedParts[wtPos] >= data->getWaveform(wtPos)->size())
		{
			selectedParts[wtPos] = data->getWaveform(wtPos)->size() - 1;
		}
		markDirty();
	}
}

std::vector<std::pair<float, float>> WavetableEditController::getVisibleAreas(
		int part)
{
	if (data)
	{
		WaveformPart* p = data->getPartByIndex(wtPos, part);
		return data->getVisibleAreas(wtPos, p);
	}
	return std::vector<std::pair<float, float>>();
}

int WavetableEditController::getVisiblePartAtPos(float p)
{
	if (data)
	{
		return data->getIndexOfPart(wtPos, data->getVisiblePartAtPos(wtPos, p));
	}
	return -1;
}

float WavetableEditController::getPartStart(int part)
{
	if (data)
	{
		WaveformPart* p = data->getPartByIndex(wtPos, part);
		if (p)
		{
			return p->getStart();
		}
	}
	return 0;
}

float WavetableEditController::getPartEnd(int part)
{
	if (data)
	{
		WaveformPart* p = data->getPartByIndex(wtPos, part);
		if (p)
		{
			return p->getEnd();
		}
	}
	return 1;
}

WaveformPart::WaveformPartType WavetableEditController::getBaseType()
{
	if (data)
	{
		return (WaveformPart::WaveformPartType) data->getPartByIndex(wtPos, 0)->getType();
	}
	return WaveformPart::SAMPLES;
}

bool WavetableEditController::replaceBase(WaveformPart* part)
{
	if (data && part)
	{
		data->removePart(wtPos, 0);
		part->setStart(0);
		part->setEnd(1);
		data->addPart(wtPos, part, 0);
		markDirty();
	}
	return false;
}

void WavetableEditController::convertToSin()
{
	std::vector<float>* samples = getSamples(getSelectedWaveform());
	std::vector<double> realSmpls(samples->begin(), samples->end());
	std::vector<double> imagSmpls(samples->size());
	if (samples)
	{
		delete samples;
	}
	samples = nullptr;

	Fft::transform(realSmpls, imagSmpls);

	/*realSmpls.erase(realSmpls.begin());
	 imagSmpls.erase(imagSmpls.begin());*/

	std::vector<WaveformPart*>* wf = data->getWaveform(wtPos);

	for (int i = 0; i < wf->size(); i++)
	{
		delete (*wf)[i];
	}
	wf->clear();

	harmonicTable_t ht;

	for (int i = 0; i < 128; i++)
	{
		double real = fabs(realSmpls[i]) < 0.001 ? 0 : realSmpls[i];
		double imag = fabs(imagSmpls[i]) < 0.001 ? 0 : imagSmpls[i];
		float amp = sqrt(real * real + imag * imag) / (realSmpls.size() / 2);
		if (i == 0)
		{
			amp = amp / 2;
		}
		float phase = atan2(-real, -imag) / (2 * M_PI);
		if (phase < 0)
		{
			phase += 1;
		}
		if (amp < 0.001)
		{
			phase = 0;
		}
		ht.push_back(std::pair<float, float>(amp, phase));
	}
	WPHarmonics* harm = new WPHarmonics(0, 1, ht, WPHarmonics::SIN);

	wf->push_back(harm);
}

void WavetableEditController::setHarmonicsType(int part, WPHarmonics::functionType type)
{
	if (data)
	{
		WaveformPart* p = data->getPartByIndex(getSelectedWaveform(), part);
		if (p && p->getType() == WaveformPart::HARMONICS)
		{
			WPHarmonics* ph = (WPHarmonics*) p;
			ph->setFunctionType(type);
		}
	}
}

void WavetableEditController::setTool(TOOL t)
{
	tool = t;
}

int WavetableEditController::getTool()
{
	return tool;
}

WTTool* WavetableEditController::getNewTool(float x, float y)
{
	switch (tool)
	{
	case TRI_SLOPE:
		return new TriTool(data, wtPos, x, y);
	case SIN_SLOPE:
		return new SinSlopeTool(data, wtPos, x, y);
	case SIN_HALF:
		return new SinHalfTool(data, wtPos, x, y);
	case FREE:
		return new FreeTool(data, wtPos, x, y);
	default:
		return nullptr;
	}
}

int WavetableEditController::getGridX()
{
	return gridX;
}

int WavetableEditController::getGridY()
{
	return gridY;
}

void WavetableEditController::setGridX(int x)
{
	gridX = x;
}

void WavetableEditController::setGridY(int y)
{
	gridY = y;
}

void WavetableEditController::selectPartAction(int x, int y, int w, int h)
{
	float rmx = (float) x / w;

	int part = getVisiblePartAtPos(rmx);
	selectPart(part);
}

void WavetableEditController::useToolAction(int x, int y, int w, int h)
{
	if (usedTool)
	{
		delete usedTool;
		usedTool = nullptr;
	}
	else
	{
		float snapX = (float) x / w;
		if (gridX > 0)
		{
			snapX = (float) ((int) (snapX * gridX + 0.5)) / gridX;
		}
		float snapY = (float) y / h;
		if (gridY > 0)
		{
			snapY = (float) ((int) (snapY * gridY + 0.5)) / gridY;
		}
		snapY = -2 * snapY + 1;
		usedTool = getNewTool(snapX, snapY);
		if (data)
		{
			selectPart(data->getIndexOfPart(wtPos, usedTool->getPart()));
		}
	}
}

void WavetableEditController::toolMoveAction(int x, int y, int w, int h)
{
	float snapX = (float) x / w;
	if (gridX > 0)
	{
		snapX = (float) ((int) (snapX * gridX + 0.5)) / gridX;
	}
	float snapY = (float) y / h;
	if (gridY > 0)
	{
		snapY = (float) ((int) (snapY * gridY + 0.5)) / gridY;
	}
	snapY = -2 * snapY + 1;

	if (usedTool)
	{
		usedTool->motion(snapX, snapY);
	}
}

void WavetableEditController::dropToolAction()
{
	if (usedTool)
	{
		delete usedTool;
		usedTool = nullptr;
	}
}

bool WavetableEditController::isDirty()
{
	return dirty;
}

void WavetableEditController::markDirty()
{
	dirty = true;
}

void WavetableEditController::clean()
{
	dirty = false;
}

} /* namespace cr42y */
