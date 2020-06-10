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


#ifndef SRC_UI_WTEDITOR_WAVEFORMVIEW_H_
#define SRC_UI_WTEDITOR_WAVEFORMVIEW_H_

#include <vector>

#include "group.hxx"

namespace Avtk
{
class Button;
}

namespace cr42y
{

class WTEditor;
class WTTool;
class CRSurfaceButton;

class WaveformView : public Avtk::Group
{
public:
	WaveformView(WTEditor* ed, int x, int y, int w, int h, std::string label);
	virtual ~WaveformView();

	virtual void draw(cairo_t* cr);

	virtual int handle(const PuglEvent* event);
	virtual void valueCB(Avtk::Widget* widget);
	
	void updateButtons();
	void requestRedraw();

private:
	WTEditor* editor;

	bool redraw;
	cairo_surface_t* surfCache;
	cairo_t* cairoCache;

	std::vector<std::pair<int, CRSurfaceButton*>> removeBtns;
	CRSurfaceButton* resizeLeft;
	CRSurfaceButton* resizeRight;
	
	CRSurfaceButton* next;
	CRSurfaceButton* prev;
	
	CRSurfaceButton* gridXPlus;
	CRSurfaceButton* gridXMinus;
	CRSurfaceButton* gridYPlus;
	CRSurfaceButton* gridYMinus;
	
	Avtk::Button* toSinBtn;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_WAVEFORMVIEW_H_ */
