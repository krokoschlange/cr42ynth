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
/*
 * CR42YConfig.cpp
 *
 *  Created on: 02.07.2020
 *      Author: fabian
 */

#include "CR42YConfig.h"

#include <sstream>
#include <vector>

namespace cr42y
{

CR42YConfig::CR42YConfig()
{
	
}

CR42YConfig::~CR42YConfig()
{
}

std::string CR42YConfig::get(std::string prop)
{
	try
	{
		return map.at(prop);
	}
	catch (std::out_of_range& e)
	{
		return "";
	}
}

int CR42YConfig::read(std::string conf)
{
	std::stringstream stream(conf);
	std::string line;

	std::string name;
	std::string param;

	while (std::getline(stream, line, '\n'))
	{
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == ' ' || line[i] == '\t')
			{
				line.erase(i, 1);
				i--;
			}
		}

		if (line.find('=') != std::string::npos)
		{
			name = line.substr(0, line.find('='));

			param = line.substr(line.find('=') + 1);

			map[name] = param;
		}
	}
}

} /* namespace cr42y */
