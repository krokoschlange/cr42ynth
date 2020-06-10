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
