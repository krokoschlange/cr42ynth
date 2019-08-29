/*
 * WavetableEditData.h
 *
 *  Created on: 07.08.2019
 *      Author: fabian
 */

#ifndef SRC_COMMON_WAVETABLEEDITDATA_H_
#define SRC_COMMON_WAVETABLEEDITDATA_H_

#include "WaveformPart.h"

namespace cr42y
{

class WavetableEditData
{
public:
	WavetableEditData(int w);
	WavetableEditData(char* data);
	virtual ~WavetableEditData();

	int getWidth();

	int getData(void** buffer);

	void addWaveform(int idx = -1);
	void setBase(int row, WaveformPart* part);
	void addPart(int row, WaveformPart* part, float end);
	void removePart(int row, WaveformPart* part);

	std::vector<WaveformPart*>* getWaveform(int row);
	WaveformPart* getBase(int row);

	float getSample(int row, int smpl);
	std::vector<float>* getSamples(int row);
	std::vector<std::vector<float>>* getSamples();

	void update(char* data);
	int getUpdate(void** buffer, int row);

	std::string to_string();

private:
	int width;

	std::vector<std::vector<WaveformPart*>> parts;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_WAVETABLEEDITDATA_H_ */
