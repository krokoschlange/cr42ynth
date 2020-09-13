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
 * LineEntry.h
 *
 *  Created on: 07.06.2020
 *      Author: fabian
 */

#ifndef SRC_DSP_LINEENTRY_H_
#define SRC_DSP_LINEENTRY_H_

#include "widget.hxx"

namespace cr42y
{

class LineEntry : public Avtk::Widget
{
public:
	LineEntry(Avtk::UI* ui, int x, int y, int w, int h, std::string lbl);
	virtual ~LineEntry();

	void setText(std::string txt);
	std::string getText();

	void setCursorPos(int pos);
	int getCursorPos();

	void setScrollPos(float pos);
	float getScrollPos();

	void setAllowedChars(std::string chars);
	std::string getAllowedChars();

	int toTextPos(cairo_t* cr, int x);
	int toPixelPos(cairo_t* cr, int pos);

	void draw(cairo_t* cr);

	int handle(const PuglEvent* event);

private:
	std::string text;
	int cursorPos;

	std::string allowedChars;

	bool focus;

	int scrollPos;

	float lastClickedPos;
};

} /* namespace cr42y */

#endif /* SRC_DSP_LINEENTRY_H_ */
