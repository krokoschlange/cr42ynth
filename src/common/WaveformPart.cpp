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

#include "WavetableEditData.h"
#include "WPFunction.h"
#include "WPHarmonics.h"
#include "WPSamples.h"

namespace cr42y
{

WaveformPart::WaveformPart(float s, float e, WaveformPartType t) :
		start(s),
		end(e),
		type(t)
{
}

/*WaveformPart::WaveformPart(char** data) :
		samples(nullptr),
		function(nullptr),
		symTable(nullptr),
		funcExpr(nullptr),
		parser(nullptr),
		var(0),
		end(0)
{
	start = *(float*) *data;
	*data += sizeof(float);
	end = *(float*) *data;
	*data += sizeof(float);
	type = *(WaveformPartType*) *data;
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
}*/

/*WaveformPart::WaveformPart(WaveformPart* part, float newStart, int size) :
		base(part->getBase()),
		start(newStart),
		type(part->getType()),
		function(part->getFunction()),
		samples(nullptr),
		symTable(nullptr),
		funcExpr(nullptr),
		parser(nullptr),
		var(0),
		end(0)
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

	if (getFunction() && getFunction()->compare("base()"))
	{
		symTable = new exprtk::symbol_table<float>();
		funcExpr = new exprtk::expression<float>();
		parser = new exprtk::parser<float>();

		symTable->add_variable("x", var);
		symTable->add_constants();

		funcExpr->register_symbol_table(*symTable);

		parser->compile(*getFunction(), *funcExpr);
	}
}*/

WaveformPart* WaveformPart::getFromData(char** data)
{
	PartDataHead* head = (PartDataHead*) *data;
	*data += sizeof(PartDataHead);
	switch (head->type)
	{
	case SAMPLES:
	{
		//return new WPSamples(head->start, head->end, data, head->size);
		return new WPSamples(head->start, head->end, data, head->size);
		break;
	}
	case FUNCTION:
	{
		return new WPFunction(head->start, head->end, data, head->size);
	}
	case HARMONICS:
	{
		return new WPHarmonics(head->start, head->end, data, head->size);
	}
	default:
		return nullptr;
	}
}

WaveformPart::~WaveformPart()
{
}

/*
int WaveformPart::getData(void** buffer)
{
	int size = 2 * sizeof(float) + sizeof(int);

	char* mem = new char[size];
	*buffer = mem;

	*(float*) mem = start;
	mem += sizeof(float);
	*(float*) mem = end;
	mem += sizeof(float);
	*(int*) mem = type;
	mem += sizeof(int);

	return size;
}*/

WaveformPart::PartDataHead* WaveformPart::getDataHead()
{
	PartDataHead* head = new PartDataHead();
	head->start = start;
	head->end = end;
	head->type = type;
	head->size = sizeof(PartDataHead) - sizeof(int);
	return head;
}

void WaveformPart::setStart(float s)
{
	start = s;
}

void WaveformPart::setEnd(float e)
{
	end = e;
}

int WaveformPart::getType()
{
	return type;
}

float WaveformPart::getStart()
{
	return start;
}

float WaveformPart::getEnd()
{
	return end;
}

std::string WaveformPart::to_string()
{
	std::string str = "WFP: ";
	str += "start = " + std::to_string(start);
	str += ", end = " + std::to_string(end);
	str += ", type = " + std::to_string(type);
	return str;
}

} /* namespace cr42y */
