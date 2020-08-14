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
