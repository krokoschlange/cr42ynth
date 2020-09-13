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
/*
 * WavetableEditController.cpp
 *
 *  Created on: 22.05.2020
 *      Author: fabian
 */

#include "WavetableEditController.h"
#include "WavetableEditData.h"
#include "WPHarmonics.h"
#include "WPFunction.h"
#include "WPSamples.h"
#include <cmath>

#include "FreeTool.h"
#include "SinHalfTool.h"
#include "SinSlopeTool.h"
#include "TriTool.h"

#include "FftRealPair.hpp"

namespace cr42y
{

WavetableEditController::WavetableEditController() :
		data_(nullptr),
		wtPos_(0),
		selectedParts_(),
		tool_(TRI_SLOPE),
		usedTool_(nullptr),
		gridX_(0),
		gridY_(0),
		editSelected_(false)
{
	
}

WavetableEditController::~WavetableEditController()
{
}

void WavetableEditController::setData(WavetableEditData* data)
{
	if (data_ != data)
	{
		data_ = data;
		if (data_)
		{
			selectedParts_.clear();
			for (int i = 0; i < data_->getWaveforms()->size(); i++)
			{
				selectedParts_.push_back(-1);
			}
		}
		else
		{
			selectedParts_.clear();
			selectedParts_.push_back(-1);
		}
		signalSelectedChangedDone_.emit();
	}
}

WavetableEditData* WavetableEditController::data()
{
	return data_;
}

int WavetableEditController::getWaveformWidth()
{
	if (data_)
	{
		return data_->getWidth();
	}
	return 2;
}

int WavetableEditController::getWavetableHeight()
{
	if (data_)
	{
		return data_->getWaveforms()->size();
	}
	return 0;
}

std::vector<float>* WavetableEditController::getSamples(int row, int stepSize)
{
	if (!data_)
	{
		std::vector<float>* ret = new std::vector<float>;
		ret->push_back(0);
		ret->push_back(0);
		return ret;
	}
	return data_->getSamples(row, stepSize);
}

std::vector<float>* WavetableEditController::getPartSamples(int stepSize)
{
	if (data_)
	{
		return data_->getPartSamples(selectedWaveform(), getSelectedPart(), stepSize);
	}
	return nullptr;
}

void WavetableEditController::selectWaveform(int num)
{
	num = num >= selectedParts_.size() ? selectedParts_.size() - 1 : num;
	num = num < 0 ? 0 : num;
	int old = wtPos_;
	wtPos_ = num;
	if (old != wtPos_)
	{
		signalSelectedChangedDone_.emit();
	}
}

int WavetableEditController::selectedWaveform()
{
	return wtPos_;
}

void WavetableEditController::selectPart(int part)
{
	if (data_)
	{
		std::vector<WaveformPart*>* wf = data_->getWaveform(wtPos_);
		if (wf)
		{
			part = part >= wf->size() ? wf->size() - 1 : part;
			part = part < -1 ? -1 : part;
			int old = selectedParts_[wtPos_];
			selectedParts_[wtPos_] = part;
			if (old != part)
			{
				signalSelectedChangedDone_.emit();
			}
		}
	}
}

int WavetableEditController::getSelectedPart()
{
	if (data_ && selectedParts_.size() > 0)
	{
		return selectedParts_[wtPos_];
	}
	else
	{
		return -1;
	}
}

int WavetableEditController::getPartAmount()
{
	if (data_)
	{
		std::vector<WaveformPart*>* wf = data_->getWaveform(selectedWaveform());
		if (wf)
		{
			return wf->size();
		}
	}
	return 0;
}

void WavetableEditController::addWaveform(int idx)
{
	if (data_)
	{
		data_->addWaveform(idx);
		if (idx < 0 || idx >= selectedParts_.size())
		{
			selectedParts_.push_back(0);
		}
		else
		{
			selectedParts_.insert(selectedParts_.begin() + idx, 0);
		}
		signalSelectedChangedDone_.emit();
	}
}

void WavetableEditController::removeWaveform(int idx, bool erase)
{
	if (data_ && idx >= 0 && idx < selectedParts_.size())
	{
		data_->removeWaveform(idx, erase);
		selectedParts_.erase(selectedParts_.begin() + idx);
		if (wtPos_ >= data_->getWaveforms()->size())
		{
			wtPos_ = data_->getWaveforms()->size() - 1;
		}

		signalSelectedChangedDone_.emit();
	}
}

void WavetableEditController::moveWaveform(int idx, int newIdx)
{
	if (data_ && idx >= 0 && idx < data_->getWaveforms()->size() && newIdx >= 0 && newIdx <= data_->getWaveforms()->size())
	{
		int oldIdx = idx + (newIdx < idx);
		data_->addWaveform(newIdx);
		if (newIdx == selectedParts_.size())
		{
			selectedParts_.push_back(selectedParts_[idx]);
		}
		else
		{
			selectedParts_.insert(selectedParts_.begin() + newIdx, selectedParts_[idx]);
		}

		data_->removePart(newIdx, 0);

		for (int i = 0; i < data_->getWaveform(oldIdx)->size(); i++)
		{
			data_->addPart(newIdx, data_->getPartByIndex(oldIdx, i));
		}
		data_->removeWaveform(oldIdx, false);
		selectedParts_.erase(selectedParts_.begin() + oldIdx);

		signalSelectedChangedDone_.emit();
	}
}

bool WavetableEditController::addPart(WaveformPart* part, int idx)
{
	if (data_)
	{
		data_->addPart(wtPos_, part, idx);
		signalSelectedChangedDone_.emit();
		return true;
	}
	return false;
}

void WavetableEditController::removePart(int idx)
{
	if (data_ && idx >= 0 && idx < data_->getWaveform(wtPos_)->size())
	{
		data_->removePart(wtPos_, idx);
		if (selectedParts_[wtPos_] >= data_->getWaveform(wtPos_)->size())
		{
			selectedParts_[wtPos_] = data_->getWaveform(wtPos_)->size() - 1;
		}
		signalSelectedChangedDone_.emit();
	}
}

void WavetableEditController::movePart(int idx, int newIdx)
{
	if (data_ && idx >= 0 && idx < data_->getWaveform(wtPos_)->size() && newIdx >= 0)
	{
		WaveformPart* part = data_->getPartByIndex(wtPos_, idx);
		if (part)
		{
			data_->removePart(wtPos_, idx, false);
			data_->addPart(wtPos_, part, newIdx);
			signalSelectedChangedDone_.emit();
		}
	}
}

std::vector<std::pair<float, float>> WavetableEditController::getVisibleAreas(
		int part)
{
	if (data_)
	{
		WaveformPart* p = data_->getPartByIndex(wtPos_, part);
		if (p)
		{
			return data_->getVisibleAreas(wtPos_, p);
		}
	}
	return std::vector<std::pair<float, float>>();
}

int WavetableEditController::getVisiblePartAtPos(float p)
{
	if (data_)
	{
		return data_->getIndexOfPart(wtPos_, data_->getVisiblePartAtPos(wtPos_, p));
	}
	return -1;
}

float WavetableEditController::getPartStart(int part)
{
	if (data_)
	{
		WaveformPart* p = data_->getPartByIndex(wtPos_, part);
		if (p)
		{
			return p->getStart();
		}
	}
	return 0;
}

float WavetableEditController::getPartEnd(int part)
{
	if (data_)
	{
		WaveformPart* p = data_->getPartByIndex(wtPos_, part);
		if (p)
		{
			return p->getEnd();
		}
	}
	return 1;
}

void WavetableEditController::resizePart(float start, float end)
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (part)
		{
			part->setStart(start);
			part->setEnd(end);

			if (part->getType() == WaveformPart::SAMPLES)
			{
				WPSamples* wpsmpls = (WPSamples*) part;
				int neededSamples = std::round((start - end) * getWaveformWidth());

				std::vector<float>* samples = wpsmpls->getSamples();

				if (samples->size() < neededSamples)
				{
					samples->resize(neededSamples - samples->size(), 0);
				}
			}
		}
	}
}

void WavetableEditController::setHarmonicsType(WPHarmonics::functionType type)
{
	if (data_)
	{
		WaveformPart* p = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (p && p->getType() == WaveformPart::HARMONICS)
		{
			WPHarmonics* ph = (WPHarmonics*) p;
			ph->setFunctionType(type);
			signalSelectedChangedDone_.emit();
		}
	}
}

int WavetableEditController::getHarmonicsType()
{
	if (data_)
	{
		WaveformPart* p = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (p && p->getType() == WaveformPart::HARMONICS)
		{
			return ((WPHarmonics*) p)->getFunctionType();
		}
	}
	return -1;
}

std::vector<std::pair<float, float>>* WavetableEditController::getHarmonicsTable()
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (part && part->getType() == WaveformPart::HARMONICS)
		{
			return ((WPHarmonics*) part)->getHarmonicTable();
		}
	}
	return nullptr;
}

void WavetableEditController::setHarmonic(int num, float amp, float phase)
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (part && part->getType() == WaveformPart::HARMONICS)
		{
			std::vector<std::pair<float, float>>* harmTable = ((WPHarmonics*) part)->getHarmonicTable();
			(*harmTable)[num].first = amp;
			(*harmTable)[num].second = phase;
			((WPHarmonics*) part)->setUpdate();
			signalSelectedChangedDone_.emit();
		}
	}
}

void WavetableEditController::normalizeHarmonic()
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (part && part->getType() == WaveformPart::HARMONICS)
		{
			((WPHarmonics*) part)->normalize();
			signalSelectedChangedDone_.emit();
		}
	}
}

bool WavetableEditController::getFunction(std::string* function)
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(wtPos_, selectedParts_[wtPos_]);
		if (part && part->getType() == WaveformPart::FUNCTION)
		{
			if (function)
			{
				*function = ((WPFunction*) part)->getFunction();
			}
			return true;
		}
	}
	return false;
}

void WavetableEditController::setFunction(std::string func)
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(wtPos_, selectedParts_[wtPos_]);
		if (part && part->getType() == WaveformPart::FUNCTION)
		{
			((WPFunction*) part)->setFunction(func);
			signalSelectedChangedDone_.emit();
		}
	}
}

void WavetableEditController::convertToSin()
{
	if (data_)
	{
		std::vector<float>* samples = getSamples(selectedWaveform());
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

		std::vector<WaveformPart*>* wf = data_->getWaveform(wtPos_);
		if (!wf)
		{
			return;
		}

		for (int i = 0; i < wf->size(); i++)
		{
			delete (*wf)[i];
		}
		wf->clear();

		std::vector<std::pair<float, float>> ht;

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

		selectedParts_[wtPos_] = 0;
		signalSelectedChangedDone_.emit();
	}
}

void WavetableEditController::replacePartWithDefault(
		WaveformPart::WaveformPartType type)
{
	if (data_)
	{
		WaveformPart* old = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (old)
		{
			WaveformPart* newPart = nullptr;
			switch (type)
			{
			case WaveformPart::FUNCTION:
			{
				newPart = new WPFunction(old->getStart(), old->getEnd(), "0");
				break;
			}
			case WaveformPart::HARMONICS:
			{
				std::vector<std::pair<float, float>> htable;
				htable.push_back(std::pair<float, float>(0, 0));
				htable.push_back(std::pair<float, float>(1, 0));
				newPart = new WPHarmonics(old->getStart(), old->getEnd(), htable);
				break;
			}
			case WaveformPart::SAMPLES:
			{
				int n_smpls = (int) std::ceil((old->getEnd() - old->getStart()) * data_->getWidth());
				std::vector<float> smpls(n_smpls, 0);
				newPart = new WPSamples(old->getStart(), old->getEnd(), smpls);
				break;
			}
			default:
				break;
			}
			if (newPart)
			{
				data_->removePart(selectedWaveform(), getSelectedPart());
				data_->addPart(selectedWaveform(), newPart, getSelectedPart());
				signalSelectedChangedDone_.emit();
			}
		}
	}
}

int WavetableEditController::getPartType()
{
	if (data_)
	{
		WaveformPart* part = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (part)
		{
			return part->getType();
		}
	}
	return -1;
}

void WavetableEditController::setTool(TOOL t)
{
	tool_ = t;
	editSelected_ = false;
}

int WavetableEditController::tool()
{
	return tool_;
}

WTTool* WavetableEditController::getNewTool(float x, float y)
{
	if (editSelected_)
	{
		WaveformPart* part = data_->getPartByIndex(selectedWaveform(), getSelectedPart());
		if (part)
		{
			switch (part->getType())
			{
			case WaveformPart::SAMPLES:
				return new FreeTool(data_, (WPSamples*) part, wtPos_, x, y);
				//add other types if we want to but i dont see a point atm
			default:
				break;
			}
		}
	}

	switch (tool_)
	{
	case TRI_SLOPE:
		return new TriTool(data_, wtPos_, x, y);
	case SIN_SLOPE:
		return new SinSlopeTool(data_, wtPos_, x, y);
	case SIN_HALF:
		return new SinHalfTool(data_, wtPos_, x, y);
	case FREE:
		return new FreeTool(data_, wtPos_, x, y);
	default:
		return nullptr;
	}
}

int WavetableEditController::gridX()
{
	return gridX_;
}

int WavetableEditController::gridY()
{
	return gridY_;
}

void WavetableEditController::setGridX(int x)
{
	gridX_ = x;
}

void WavetableEditController::setGridY(int y)
{
	gridY_ = y;
}

void WavetableEditController::selectPartAction(int x, int y, int w, int h)
{
	float rmx = (float) x / w;

	int part = getVisiblePartAtPos(rmx);
	if (getSelectedPart() != part)
	{
		selectPart(part);
	}
	else
	{
		selectPart(-1);
	}
}

void WavetableEditController::useToolAction(int x, int y, int w, int h)
{
	if (usedTool_)
	{
		delete usedTool_;
		usedTool_ = nullptr;
		signalSelectedChangedDone_.emit();
	}
	else
	{
		x = std::min<int>(w, std::max<int>(x, 0));
		y = std::min<int>(h, std::max<int>(y, 0));
		float snapX = (float) x / w;
		if (gridX_ > 0)
		{
			snapX = (float) ((int) (snapX * gridX_ + 0.5)) / gridX_;
		}
		float snapY = (float) y / h;
		if (gridY_ > 0)
		{
			snapY = (float) ((int) (snapY * gridY_ + 0.5)) / gridY_;
		}
		snapY = -2 * snapY + 1;
		usedTool_ = getNewTool(snapX, snapY);
		if (data_)
		{
			selectPart(data_->getIndexOfPart(wtPos_, usedTool_->getPart()));
		}
		signalSelectedChanged_.emit();
	}
}

void WavetableEditController::toolMoveAction(int x, int y, int w, int h)
{
	x = std::min<int>(w, std::max<int>(x, 0));
	y = std::min<int>(h, std::max<int>(y, 0));
	float snapX = (float) x / w;
	if (gridX_ > 0)
	{
		snapX = (float) ((int) (snapX * gridX_ + 0.5)) / gridX_;
	}
	float snapY = (float) y / h;
	if (gridY_ > 0)
	{
		snapY = (float) ((int) (snapY * gridY_ + 0.5)) / gridY_;
	}
	snapY = -2 * snapY + 1;

	if (usedTool_)
	{
		usedTool_->motion(snapX, snapY);
		signalSelectedChanged_.emit();
	}
}

void WavetableEditController::dropToolAction()
{
	if (usedTool_)
	{
		delete usedTool_;
		usedTool_ = nullptr;
		editSelected_ = false;
		signalSelectedChangedDone_.emit();
	}
}

void WavetableEditController::setEditSelected(bool editSelected)
{
	editSelected_ = editSelected;
}

bool WavetableEditController::getEditSelected()
{
	return editSelected_;
}

sigc::signal<void> WavetableEditController::signalSelectedChanged()
{
	return signalSelectedChanged_;
}

sigc::signal<void> WavetableEditController::signalSelectedChangedDone()
{
	return signalSelectedChangedDone_;
}

} /* namespace cr42y */
