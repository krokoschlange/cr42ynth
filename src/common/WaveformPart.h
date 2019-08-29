/*
 * WaveformPart.h
 *
 *  Created on: 07.08.2019
 *      Author: fabian
 */

#ifndef SRC_COMMON_WAVEFORMPART_H_
#define SRC_COMMON_WAVEFORMPART_H_

#include <string>
#include <vector>

#include "exprtk.hpp"

namespace cr42y
{

enum WaveformPartType {
	SAMPLES,
	FUNCTION
};

class WaveformPart
{
public:
	WaveformPart(float s, int t, bool b = false, std::string* func = nullptr, std::vector<float>* sam = nullptr);
	WaveformPart(char** data);
	WaveformPart(WaveformPart* part, float newStart, int size);
	virtual ~WaveformPart();

	int getData(void** buffer);

	float getSample(int size, int pos);

	void setType(int t);
	void setStart(float s);
	void setFunction(std::string* func);


	float getStart();
	int getType();
	std::string* getFunction();
	std::vector<float>* getSamples();

	bool getBase();

	std::string to_string();

private:
	bool base;

	float start;
	int type;
	std::string* function;
	std::vector<float>* samples;

	exprtk::symbol_table<float>* symTable;
	exprtk::expression<float>* funcExpr;
	exprtk::parser<float>* parser;
	float var;

};

} /* namespace cr42y */

#endif /* SRC_COMMON_WAVEFORMPART_H_ */
