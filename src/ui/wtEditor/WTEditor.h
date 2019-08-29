/*
 * WTEditor.h
 *
 *  Created on: 10.08.2019
 *      Author: fabian
 */

#ifndef SRC_UI_WTEDITOR_WTEDITOR_H_
#define SRC_UI_WTEDITOR_WTEDITOR_H_

#include "group.hxx"

#include "WavetableEditData.h"
#include "WaveformView.h"

namespace cr42y
{

class WTEditor : public Avtk::Group
{
public:
	WTEditor(Avtk::UI* ui, int x, int y, int w, int h, std::string label, int s);
	virtual ~WTEditor();

	void setOscillator(int num);

	void setWTPos(int pos);

	WavetableEditData* getEditData();
	int getWTPos();

private:
	int wtPos;
	WavetableEditData* data;

	WaveformView* view;
};

} /* namespace cr42y */

#endif /* SRC_UI_WTEDITOR_WTEDITOR_H_ */
