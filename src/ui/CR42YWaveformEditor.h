/*
 * CR42YWaveformEditor.h
 *
 *  Created on: 04.08.2020
 *      Author: fabian
 */

#ifndef SRC_UI_CR42YWAVEFORMEDITOR_H_
#define SRC_UI_CR42YWAVEFORMEDITOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class WavetableEditController;

class CR42YWaveformEditor : public CR42YRelativeContainer
{
public:
	CR42YWaveformEditor(CR42YUI* ui);
	virtual ~CR42YWaveformEditor();

	void setController(WavetableEditController* controller);

	sigc::signal<void> waveformUpdateSignal();

	void updateButtons();

protected:
	void on_realize();

	bool on_expose_event(GdkEventExpose* event);

	bool on_button_press(GdkEventButton* event);
	bool on_button_release(GdkEventButton* event);
	bool on_motion_notify(GdkEventMotion* event);

	void removePartCallback(int part);

private:
	Glib::RefPtr<Gdk::Window> window_;
	sigc::signal<void> waveformUpdateSignal_;


	WavetableEditController* controller_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YWAVEFORMEDITOR_H_ */
