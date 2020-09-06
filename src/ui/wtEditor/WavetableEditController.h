/*
 * WavetableEditController.h
 *
 *  Created on: 22.05.2020
 *      Author: fabian
 */

#ifndef SRC_UI_WTEDITOR_WAVETABLEEDITCONTROLLER_H_
#define SRC_UI_WTEDITOR_WAVETABLEEDITCONTROLLER_H_

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

	void setData(WavetableEditData* data);
	WavetableEditData* data();

	int getWaveformWidth();
	int getWavetableHeight();
	std::vector<float>* getSamples(int row, int stepSize = 1);

	void selectWaveform(int num);
	int selectedWaveform();

	void selectPart(int part);
	int getSelectedPart();

	void addWaveform(int idx);
	void removeWaveform(int idx, bool erase = true);

	void moveWaveform(int idx, int newIdx);

	bool addPart(WaveformPart* part, int idx = -1);
	void removePart(int idx);
	void movePart(int idx, int newIdx);

	std::vector<std::pair<float, float>> getVisibleAreas(int part);
	int getVisiblePartAtPos(float p);

	float getPartStart(int part);
	float getPartEnd(int part);

	WaveformPart::WaveformPartType getBaseType();
	bool replaceBase(WaveformPart* part);

	void setHarmonicsType(int part, WPHarmonics::functionType type);
	std::vector<std::pair<float, float>>* getHarmonicsTable();
	void setHarmonic(int num, float amp, float phase);
	void normalizeHarmonic();

	bool getFunction(std::string* function);
	void setFunction(std::string func);

	void convertToSin();

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

	sigc::signal<void> signalSelectedChanged();
	sigc::signal<void> signalSelectedChangedDone();

private:
	WavetableEditData* data_;

	sigc::signal<void> signalSelectedChanged_;
	sigc::signal<void> signalSelectedChangedDone_;

	TOOL tool_;
	WTTool* usedTool_;
	int gridX_;
	int gridY_;

	int wtPos_;
	std::vector<int> selectedParts_;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_WAVETABLEEDITCONTROLLER_H_ */
