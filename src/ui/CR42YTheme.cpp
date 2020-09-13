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
 * CR42YTheme.cpp
 *
 *  Created on: 28.06.2020
 *      Author: fabian
 */

#include "CR42YTheme.h"

#include <sstream>

#include "CR42YConfig.h"

namespace cr42y
{

CR42YTheme::CR42YTheme(std::string theme)
{
	load(theme);
}

CR42YTheme::~CR42YTheme()
{
}

float* CR42YTheme::color(CR42YCOLORS color)
{
	return colors_[color];
}

float CR42YTheme::lineThin()
{
	return lineThin_;
}

float CR42YTheme::lineMiddle()
{
	return lineMiddle_;
}

float CR42YTheme::lineThick()
{
	return lineThick_;
}

float CR42YTheme::cornerRadius()
{
	return cornerRadius_;
}

std::string CR42YTheme::font()
{
	return font_;
}

int CR42YTheme::fontSizeSmall()
{
	return fontSizeSmall_;
}

int CR42YTheme::fontSizeMiddle()
{
	return fontSizeMiddle_;
}

int CR42YTheme::fontSizeBig()
{
	return fontSizeBig_;
}

void CR42YTheme::load(std::string theme)
{
	CR42YConfig config;
	config.read(theme);

	loadRGBA(config.get("fg"), colors_[FG]);
	loadRGBA(config.get("fg_dark"), colors_[FG_DARK]);
	loadRGBA(config.get("bg"), colors_[BG]);
	loadRGBA(config.get("bg_dark"), colors_[BG_DARK]);
	loadRGBA(config.get("highlight"), colors_[HIGHLIGHT]);

	std::stringstream stream;

	stream << config.get("line_thin");
	stream >> lineThin_;
	stream.clear();
	stream << config.get("line_middle");
	stream >> lineMiddle_;
	stream.clear();
	stream << config.get("line_thick");
	stream >> lineThick_;
	stream.clear();
	stream << config.get("corner_radius");
	stream >> cornerRadius_;
	stream.clear();

	font_ = config.get("font");

	stream << config.get("font_size_small");
	stream >> fontSizeSmall_;
	stream.clear();
	stream << config.get("font_size_middle");
	stream >> fontSizeMiddle_;
	stream.clear();
	stream << config.get("font_size_big");
	stream >> fontSizeBig_;
	stream.clear();
}

void CR42YTheme::loadRGBA(std::string hexColor, float* out)
{
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
	out[3] = 1;
	if (hexColor.size() >= 7 && hexColor.at(0) == '#')
	{
		std::stringstream stream;
		int val;
		for (int i = 0; i < 4 && i * 2 + 1 < hexColor.size(); i++)
		{
			stream.clear();
			stream << std::hex << hexColor.substr(i * 2 + 1, 2);
			stream >> val;

			*(out + i) = val / 255.0;
		}
	}
}

} /* namespace cr42y */
