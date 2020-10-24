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
	
	WavetableEditData& operator=(const WavetableEditData& data);

	WaveformPart* getPartByIndex(int row, int idx);
	int getIndexOfPart(int row, WaveformPart* part);
	std::vector<std::pair<float, float>> getVisibleAreas(int row, WaveformPart* part);
	WaveformPart* getVisiblePartAtPos(int row, float pos);

	int getWidth();

	int getData(void** buffer);

	void addWaveform(int idx = -1);
	void removeWaveform(int idx, bool erase = true);
	void addPart(int row, WaveformPart* part, int idx = -1);
	void removePart(int row, WaveformPart* part);
	void removePart(int row, int idx, bool erase = true);

	std::vector<std::vector<WaveformPart*>>* getWaveforms();
	std::vector<WaveformPart*>* getWaveform(int row);

	float getSample(int row, int smpl);
	std::vector<float>* getSamples(int row, int stepSize = 1);
	std::vector<std::vector<float>>* getSamples();

	std::vector<float>* getPartSamples(int row, int part, int stepSize = 1);

	void update(char* data);
	int getUpdate(void** buffer, int row);

	std::string to_string();

private:
	int width;

	std::vector<std::vector<WaveformPart*>> parts;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_WAVETABLEEDITDATA_H_ */
