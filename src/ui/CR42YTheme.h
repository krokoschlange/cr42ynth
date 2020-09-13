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
 * CR42YTheme.h
 *
 *  Created on: 28.06.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YTHEME_H_
#define SRC_UI_CR42YTHEME_H_

#include <string>

namespace cr42y
{

enum CR42YCOLORS
{
	FG,
	FG_DARK,
	BG,
	BG_DARK,
	HIGHLIGHT,

	COLOR_COUNT
};

class CR42YTheme
{
public:
	CR42YTheme(std::string theme);
	virtual ~CR42YTheme();

	float* color(CR42YCOLORS color);
	float lineThin();
	float lineMiddle();
	float lineThick();
	float cornerRadius();

	std::string font();
	int fontSizeSmall();
	int fontSizeMiddle();
	int fontSizeBig();


private:
	void load(std::string theme);
	void loadRGBA(std::string hexColor, float* out);


	float colors_[COLOR_COUNT][4];

	float lineThin_;
	float lineMiddle_;
	float lineThick_;
	float cornerRadius_;

	std::string font_;
	int fontSizeSmall_;
	int fontSizeMiddle_;
	int fontSizeBig_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YTHEME_H_ */
