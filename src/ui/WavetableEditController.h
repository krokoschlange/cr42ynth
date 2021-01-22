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


#ifndef SRC_UI_WTEDITOR_WAVETABLEEDITCONTROLLER_H_
#define SRC_UI_WTEDITOR_WAVETABLEEDITCONTROLLER_H_

#include <deque>
#include <vector>
#include <sigc++/sigc++.h>

#include "WaveformPart.h"
#include "WPHarmonics.h"

namespace cr42y
{

class WavetableEditData;
class WTTool;

class WavetableEditController
{
public:
	WavetableEditController();
	virtual ~WavetableEditController();

	void setData(WavetableEditData* data, bool eraseHistory = true);
	WavetableEditData* data();

	int getWaveformWidth();
	int getWavetableHeight();
	void getSamples(std::vector<float>& samples, int row, int stepSize = 1);

	void getPartSamples(std::vector<float>& samples, int stepSize = 1);

	void selectWaveform(int num);
	int selectedWaveform();

	void selectPart(int part);
	int getSelectedPart();

	int getPartAmount();

	void addWaveform(int idx);
	void removeWaveform(int idx, bool erase = true);

	void moveWaveform(int idx, int newIdx);

	void addFunctionWaveforms(int idx, int amnt, std::string function);
	void addWavWaveforms(int idx, int amnt, int width, std::string filepath);

	void crossfadeWaveforms(int idx, int amnt);
	void spectralFadeWaveforms(int idx, int amnt, bool zeroAll, bool zeroFundamental);

	bool addPart(WaveformPart* part, int idx = -1);
	void removePart(int idx);
	void movePart(int idx, int newIdx);

	std::vector<std::pair<float, float>> getVisibleAreas(int part);
	int getVisiblePartAtPos(float p);

	float getPartStart(int part);
	float getPartEnd(int part);

	void resizePart(float start, float end);

	void setHarmonicsType(WPHarmonics::functionType type);
	int getHarmonicsType();
	std::vector<std::pair<float, float>>* getHarmonicsTable();
	void setHarmonic(int num, float amp, float phase, bool end);
	void normalizeHarmonic();

	bool getFunction(std::string* function);
	void setFunction(std::string func);

	std::vector<std::pair<float, float>> getWFHarmonics(int idx, bool highQuality);
	void convertToSin(bool highQuality);

	void replacePartWithDefault(WaveformPart::WaveformPartType type);
	int getPartType();

	enum TOOL
	{
		TRI_SLOPE, SIN_SLOPE, SIN_HALF, FREE
	};

	void setTool(TOOL t);
	int tool();
	WTTool* getNewTool(float x, float y);


	int gridX();
	int gridY();
	void setGridX(int x);
	void setGridY(int y);

	void selectPartAction(int x, int y, int w, int h);
	void useToolAction(int x, int y, int w, int h);
	void toolMoveAction(int x, int y, int w, int h);
	void dropToolAction();

	void setEditSelected(bool editSelected);
	bool getEditSelected();

	sigc::signal<void> signalSelectedChanged();
	sigc::signal<void> signalSelectedChangedDone();
	sigc::signal<void> signalDataChanged();
	sigc::signal<void, std::deque<std::pair<char*, std::vector<int>>>> signalHistoryDelete();

	void addHistoryPoint();
	void deleteHistory();
	bool undoPossible();
	void undo();
	bool redoPossible();
	void redo();

private:
	WavetableEditData* data_;

	sigc::signal<void> signalSelectedChanged_;
	sigc::signal<void> signalSelectedChangedDone_;

	TOOL tool_;
	WTTool* usedTool_;
	bool editSelected_;
	int gridX_;
	int gridY_;

	int wtPos_;
	std::vector<int> selectedParts_;

	sigc::signal<void> signalDataChanged_;

	std::deque<std::pair<char*, std::vector<int>>> editHistory_;
	size_t historyIndex_;
	size_t historySize_;
	sigc::signal<void, std::deque<std::pair<char*, std::vector<int>>>> signalHistoryDelete_;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_WAVETABLEEDITCONTROLLER_H_ */
