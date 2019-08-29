/*
 * WaveformPart.cpp
 *
 *  Created on: 07.08.2019
 *      Author: fabian
 */

#include "WaveformPart.h"

namespace cr42y
{

WaveformPart::WaveformPart(float s, int t, bool b, std::string* func,
		std::vector<float>* sam) :
		base(b),
		start(s),
		type(t),
		function(func),
		samples(sam),
		symTable(nullptr),
		funcExpr(nullptr),
		parser(nullptr),
		var(0)
{
	if (func)
	{
		symTable = new exprtk::symbol_table<float>();
		funcExpr = new exprtk::expression<float>();
		parser = new exprtk::parser<float>();

		symTable->add_variable("x", var);
		symTable->add_constants();

		funcExpr->register_symbol_table(*symTable);

		parser->compile(*func, *funcExpr);
	}
}

WaveformPart::WaveformPart(char** data) :
		samples(nullptr),
		function(nullptr),
		base(false),
		symTable(nullptr),
		funcExpr(nullptr),
		parser(nullptr),
		var(0)
{
	start = *(float*) *data;
	*data += sizeof(float);
	base = *(bool*) *data;
	*data += sizeof(bool);
	type = *(int*) *data;
	*data += sizeof(int);
	int size = *(int*) *data;
	*data += sizeof(int);
	switch (type)
	{
	case SAMPLES:
		samples = new std::vector<float>;
		for (int i = 0; i < size; *data += sizeof(float), i += sizeof(float))
		{
			samples->push_back(*(float*) *data);
		}
		break;
	case FUNCTION:
		function = new std::string((char*) *data);
		*data += function->size() + 1;

		symTable = new exprtk::symbol_table<float>();
		funcExpr = new exprtk::expression<float>();
		parser = new exprtk::parser<float>();

		symTable->add_variable("x", var);
		symTable->add_constants();

		funcExpr->register_symbol_table(*symTable);

		parser->compile(*function, *funcExpr);
		break;
	default:
		break;
	}
}

WaveformPart::WaveformPart(WaveformPart* part, float newStart, int size) :
		base(part->getBase()),
		start(newStart),
		type(part->getType()),
		function(part->getFunction()),
		samples(nullptr),
		symTable(nullptr),
		funcExpr(nullptr),
		parser(nullptr),
		var(0)
{
	if (getBase())
	{
		function = new std::string("base()");
	}
	if (part->getSamples())
	{
		std::vector<float>* smpls = part->getSamples();
		std::vector<float>* newSamples = new std::vector<float>();

		int oldStart = part->getStart() * size;
		int newStart = getStart() * size;

		for (int i = newStart - oldStart; i < smpls->size(); i++)
		{
			newSamples->push_back((*smpls)[i]);
		}
		samples = newSamples;
	}

	if (getFunction())
	{
		symTable = new exprtk::symbol_table<float>();
		funcExpr = new exprtk::expression<float>();
		parser = new exprtk::parser<float>();

		symTable->add_variable("x", var);
		symTable->add_constants();

		funcExpr->register_symbol_table(*symTable);

		parser->compile(*getFunction(), *funcExpr);
	}
}

WaveformPart::~WaveformPart()
{
	if (function)
	{
		delete function;
		function = nullptr;
	}
	if (samples)
	{
		delete samples;
		samples = nullptr;
	}
	if (symTable)
	{
		delete symTable;
		symTable = nullptr;
	}
	if (funcExpr)
	{
		delete funcExpr;
		funcExpr = nullptr;
	}
	if (parser)
	{
		delete parser;
		parser = nullptr;
	}
}

int WaveformPart::getData(void** buffer)
{
	int totalSize = sizeof(float) + 2 * sizeof(int) + sizeof(bool);
	switch (type)
	{
	case SAMPLES:
		totalSize += samples->size() * sizeof(float);
		break;
	case FUNCTION:
		totalSize += function->size() + 1;
		break;
	default:
		return 0;
	}
	char* mem = new char[totalSize];
	*buffer = mem;

	*(float*) mem = start;
	mem += sizeof(float);
	*(bool*) mem = base;
	mem += sizeof(bool);
	*(int*) mem = type;
	mem += sizeof(int);
	*(int*) mem = totalSize - sizeof(float) - 2 * sizeof(int) - sizeof(bool);
	mem += sizeof(int);

	switch (type)
	{
	case SAMPLES:
		for (int i = 0; i < samples->size(); i++)
		{
			*(float*) mem = (*samples)[i];
			mem += sizeof(float);
		}
		break;
	case FUNCTION:
	{
		const char* c = function->c_str();
		for (int i = 0; c[i]; *mem = c[i], mem++, i++)
		{
		}
		break;
	}
	default:
		return 0;
	}
	return totalSize;
}

void WaveformPart::setType(int t)
{
	type = t;
}

void WaveformPart::setStart(float s)
{
	start = s;
}

void WaveformPart::setFunction(std::string* func)
{
	if (function)
	{
		delete function;
	}
	function = func;
}

int WaveformPart::getType()
{
	return type;
}

float WaveformPart::getStart()
{
	return start;
}

std::string* WaveformPart::getFunction()
{
	return function;
}

std::vector<float>* WaveformPart::getSamples()
{
	return samples;
}

float WaveformPart::getSample(int size, int pos)
{
	switch (type)
	{
	case SAMPLES:
	{
		int startsample = (int) (size * start);
		int smpl = pos - startsample;
		if (samples && smpl >= 0 && smpl < samples->size())
		{
			return (*samples)[smpl];
		}
		break;
	}
	case FUNCTION:
		var = pos / (float) size;
		return funcExpr->value();
		break;
	default:
		break;
	}
	return 0;
}

bool WaveformPart::getBase()
{
	return base;
}

std::string WaveformPart::to_string()
{
	std::string str = "WFP: [";
	str += "base = " + std::to_string(base);
	str += ", start = " + std::to_string(start);
	str += ", type = ";
	if (type == FUNCTION)
	{
		str += "FUNCTION: func = " + *function;
	}
	else if (type == SAMPLES)
	{
		str += "SAMPLES";
	}
	str += "]";
	return str;
}

} /* namespace cr42y */
