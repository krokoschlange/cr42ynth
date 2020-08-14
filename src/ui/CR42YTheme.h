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
