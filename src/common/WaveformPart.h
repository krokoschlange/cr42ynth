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

#ifndef SRC_COMMON_WAVEFORMPART_H_
#define SRC_COMMON_WAVEFORMPART_H_

#include <string>
#include <vector>


namespace cr42y
{

class WavetableEditData;

class WaveformPart
{
public:
	enum WaveformPartType {
		SAMPLES,
		FUNCTION,
		HARMONICS
	};
	
	typedef struct {
		float start;
		float end;
		int type;
		int size;
	} PartDataHead;
	
	//WaveformPart(float s, float e, WaveformPartType t, std::string* func = nullptr, std::vector<float>* sam = nullptr);
	WaveformPart(float s, float e, WaveformPartType t);
	//WaveformPart(char** data);
	//WaveformPart(WaveformPart* part, float newStart, int size);
	virtual ~WaveformPart();
	static WaveformPart* getFromData(char** data);

	PartDataHead* getDataHead();
	virtual int getData(void** buffer) = 0;

	virtual float getSample(int size, int pos, int ypos) = 0;

	void setStart(float s);
	void setEnd(float e);
	//void setFunction(std::string* func);


	float getStart();
	float getEnd();
	int getType();
	//std::string* getFunction();
	//std::vector<float>* getSamples();

	virtual std::string to_string();

private:
	float start;
	float end;
	WaveformPartType type;
	/*std::string* function;
	std::vector<float>* samples;

	exprtk::symbol_table<float>* symTable;
	exprtk::expression<float>* funcExpr;
	exprtk::parser<float>* parser;
	float var;*/

};

} /* namespace cr42y */

#endif /* SRC_COMMON_WAVEFORMPART_H_ */
