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

#ifndef SRC_UI_CR42YLABEL_H_
#define SRC_UI_CR42YLABEL_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YLabel : public Gtk::Widget, public CR42YWidget
{
public:
	CR42YLabel(CR42YUI* ui);
	virtual ~CR42YLabel();

	std::string text();
	Cairo::RefPtr<Cairo::Surface> surfActive();
	Cairo::RefPtr<Cairo::Surface> surfInactive();

	void setText(std::string text);
	void setSurfActive(Cairo::RefPtr<Cairo::Surface> surfActive);
	void setSurfInactive(Cairo::RefPtr<Cairo::Surface> surfInactive);

	void setFontSize(int size);

	void setForcedSizeRatio(double forcedSizeRatio);

protected:

	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void on_realize();
	virtual void on_size_allocate(Gtk::Allocation& alloc);

private:

	void drawSurface(Cairo::RefPtr<Cairo::Context> cr,
			Cairo::RefPtr<Cairo::Surface> surf);

	std::string text_;
	int fontSize_;

	double forcedSizeRatio_;

	Cairo::RefPtr<Cairo::Surface> surfActive_;
	Cairo::RefPtr<Cairo::Surface> surfInactive_;

	Glib::RefPtr<Gdk::Window> window_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YLABEL_H_ */
