/*
 * WaveformView.h
 *
 *  Created on: 12.08.2019
 *      Author: fabian
 */

#ifndef SRC_UI_WTEDITOR_WAVEFORMVIEW_H_
#define SRC_UI_WTEDITOR_WAVEFORMVIEW_H_

#include "avtk.hxx"

#include "WTEditor.h"
#include "WaveformPart.h"

namespace cr42y
{

class WaveformView : public Avtk::Widget
{
public:
	WaveformView(WTEditor* ed, Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~WaveformView();

	virtual void draw(cairo_t* cr);

	void notifyRedraw();

private:
	WTEditor* editor;

	bool redraw;
	cairo_surface_t* surfCache;
	cairo_t* cairoCache;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_WAVEFORMVIEW_H_ */
