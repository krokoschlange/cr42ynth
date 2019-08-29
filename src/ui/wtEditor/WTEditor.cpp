/*
 * WTEditor.cpp
 *
 *  Created on: 10.08.2019
 *      Author: fabian
 */

#include <iostream>

#include "WTEditor.h"

namespace cr42y
{

WTEditor::WTEditor(Avtk::UI* ui, int x, int y, int w, int h, std::string label, int s) :
		Avtk::Group(ui, x, y, w, h, label),
		wtPos(0),
		data(new WavetableEditData(32)),
		view(nullptr)//new WaveformView(this, parent_, 0, 0, 500, 500, "VIEW"))
{
	std::cout << "wut";
	data = new WavetableEditData(32);
}

WTEditor::~WTEditor()
{
}

WavetableEditData* WTEditor::getEditData()
{
	return data;
}

int WTEditor::getWTPos()
{
	return wtPos;
}

void WTEditor::setWTPos(int pos)
{
	wtPos = pos;
}

} /* namespace cr42y */
