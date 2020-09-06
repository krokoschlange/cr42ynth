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

WPFunction::WPFunction(float s, float e, std::string function) :
		WaveformPart(s, e, WaveformPart::FUNCTION),
		function_(function),
		varX_(0),
		varY_(0)
{
	symTable_.add_variable("x", varX_);
	symTable_.add_constants();
	
	funcExpr_.register_symbol_table(symTable_);
	
	setFunction(function_);
}

WPFunction::WPFunction(float s, float e, char** data, int size) :
		WaveformPart(s, e, WaveformPart::FUNCTION),
		function_(*data),
		varX_(0),
		varY_(0)
{
	*data += size;
	symTable_.add_variable("x", varX_);
	symTable_.add_variable("y", varY_);
	symTable_.add_constants();
	
	funcExpr_.register_symbol_table(symTable_);
	
	setFunction(function_);
}

WPFunction::~WPFunction()
{
}

int WPFunction::getData(void** buffer)
{
	PartDataHead* head = WaveformPart::getDataHead();
	int size = function_.size() + 1;
	head->size = size;
	
	int totalSize = sizeof(PartDataHead) + size;
	
	char* mem = new char[totalSize];
	*buffer = mem;
	
	memcpy(mem, head, sizeof(PartDataHead));
	mem += sizeof(PartDataHead);
	
	const char* c = function_.c_str();
	for (int i = 0; c[i]; *mem = c[i], mem++, i++) {}
	*mem = 0;
	mem++;
	
	delete head;
	
	return totalSize;
}

float WPFunction::getSample(int size, int pos, int ypos)
{
	varX_ = (float) pos / size;
	varY_ = ypos;
	return funcExpr_.value();
}

std::string WPFunction::to_string()
{
	std::string str = WaveformPart::to_string();
	str += " (FUNCTION): " + function_;
	return str;
}

void WPFunction::setFunction(std::string function)
{
	function_ = function;
	bool err = parser_.compile(function_, funcExpr_);
	if (!err) {
		parser_.compile("0", funcExpr_);
	}
}

std::string WPFunction::getFunction()
{
	return function_;
}

} /* namespace cr42y */
