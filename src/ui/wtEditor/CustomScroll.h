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

#ifndef SRC_UI_WTEDITOR_CUSTOMSCROLL_H_
#define SRC_UI_WTEDITOR_CUSTOMSCROLL_H_

#include <vector>

#include "group.hxx"


namespace cr42y
{

class CustomScroll : public Avtk::Group
{
public:
	CustomScroll(Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~CustomScroll();
	
	virtual void draw(cairo_t* cr);
	virtual int handle(const PuglEvent* event);
	
	void setChild(Avtk::Widget* c);
	
	void childResize();
	
	void offsetEvent(const PuglEvent* in, PuglEvent* out);
	
protected:
	Avtk::Widget* child;
	
	int mx;
	int my;
	
	enum GRAB
	{
		GRAB_NONE,
		GRAB_VERTICAL,
		GRAB_HORIZONTAL
	};
	
	GRAB grab;
	
	bool doScrollX; // if scrolling
	bool doScrollY;
	
	float scrollX; // relative amount
	float scrollY;
	
	int scrollXAmount; // scrolled amount of child in pixels
	int scrollYAmount;
	
	int maxScrollX; // maximum scrollable amount of child in px
	int maxScrollY;
	
	int scrollXRange; // range of scrollbar slider in px
	int scrollYRange;
	
	int scrollXSize; // scrollbar slider size
	int scrollYSize;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_CUSTOMSCROLL_H_ */
