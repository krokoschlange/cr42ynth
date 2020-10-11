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

#ifndef SRC_UI_CR42YDIAL_H_
#define SRC_UI_CR42YDIAL_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YDial : public Gtk::Widget, public CR42YWidget
{
public:
	CR42YDial(CR42YUI* ui);
	virtual ~CR42YDial();

	double value();
	void setValue(double value, bool trigger = true);

	double defaultValue();
	void setDefaultValue(double defaultValue);

	std::string text();
	void setText(std::string text);


	sigc::signal<void, double> signalChanged();
	sigc::signal<void, double> signalDone();

protected:

	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_realize();

	bool on_button_press(GdkEventButton* event);
	bool on_button_release(GdkEventButton* event);
	bool on_motion_notify(GdkEventMotion* event);

private:
	Glib::RefPtr<Gdk::Window> window_;

	sigc::signal<void, double> signalChanged_;
	sigc::signal<void, double> signalDone_;

	double value_;
	double preClickValue_;
	double prePreClickValue_;
	int mouseY_;

	double oldValue_;
	double defaultValue_;

	std::string text_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YDIAL_H_ */
