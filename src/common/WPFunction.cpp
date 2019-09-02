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

#include <iostream>

#include "WPFunction.h"

namespace cr42y
{

WPFunction::WPFunction(float s, float e, std::string func) :
		WaveformPart(s, e, WaveformPart::FUNCTION),
		function(func)
{
	symTable.add_variable("x", var);
	symTable.add_constants();
	
	funcExpr.register_symbol_table(symTable);
	
	parser.compile(function, funcExpr);
}

WPFunction::WPFunction(float s, float e, char** data, int size) :
		WaveformPart(s, e, WaveformPart::FUNCTION),
		function(*data)
{
	*data += size;
}

WPFunction::~WPFunction()
{
}

int WPFunction::getData(void** buffer)
{
	PartDataHead* head = WaveformPart::getDataHead();
	int size = function.size() + 1;
	head->size = size;
	
	int totalSize = sizeof(PartDataHead) + size;
	
	char* mem = new char[totalSize];
	*buffer = mem;
	
	memcpy(mem, head, sizeof(PartDataHead));
	mem += sizeof(PartDataHead);
	
	const char* c = function.c_str();
	for (int i = 0; c[i]; *mem = c[i], mem++, i++) {}
	*mem = 0;
	mem++;
	
	delete head;
	
	return totalSize;
}

float WPFunction::getSample(int size, int pos)
{
	var = (float) pos / size;
	return funcExpr.value();
}

std::string WPFunction::to_string()
{
	std::string str = WaveformPart::to_string();
	str += " (FUNCTION): " + function;
	return str;
}

void WPFunction::setFunction(std::string func)
{
	function = func;
	parser.compile(function, funcExpr);
}

std::string WPFunction::getFunction()
{
	return function;
}

} /* namespace cr42y */
