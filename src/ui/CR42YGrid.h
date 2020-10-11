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
#ifndef SRC_CR42YGRID_H_
#define SRC_CR42YGRID_H_

#include <gtkmm.h>
#include "CR42YWidget.h"

namespace cr42y
{

class CR42YGrid : public Gtk::Container, public CR42YWidget
{
public:
	CR42YGrid(CR42YUI* ui);
	virtual ~CR42YGrid();

	void addRow(double weight, double padFront, double padBack,
			int minSize, int maxSize);
	void addColumn(double weight, double padFront,
			double padBack, int minSize, int maxSize);

	void removeRow(int row);
	void removeColumn(int column);

	void configureRow(int row, double weight, double padFront, double padBack,
			int minSize, int maxSize);
	void configureColumn(int column, double weight, double padFront,
			double padBack, int minSize, int maxSize);

	void setDrawBorder(bool drawBorder);
	void setDrawBG(bool drawBG);

	enum STICKY_SIDES
	{
		NONE = 0,
		N = 8,
		E = 4,
		S = 2,
		W = 1,
		NE = 12,
		NS = 10,
		NW = 9,
		EW = 5,
		SE = 6,
		SW = 3,
		NEW = 13,
		NSE = 14,
		NSW = 11,
		SEW = 7,
		NSEW = 15
	};

	void put(Gtk::Widget* widget, int row, int column, int rowSpan = 1,
			int columnSpan = 1, int stickySides = NSEW);

	void on_remove(Gtk::Widget* child);
	void on_add(Gtk::Widget* child);

protected:
	void on_realize();
	bool on_expose_event(GdkEventExpose* event);
	void on_size_allocate(Gtk::Allocation& alloc);
	void on_size_request(Gtk::Requisition* requisition);
	void forall_vfunc(gboolean include_internals, GtkCallback callback,
			gpointer callback_data);

	struct CR42YGridRow
	{
		double weight_;
		double padFront_;
		double padBack_;
		int minSize_;
		int maxSize_;
		int allocated_;
	};

	std::vector<CR42YGridRow> rows_;
	std::vector<CR42YGridRow> columns_;

	struct CR42YGridChild
	{
		Gtk::Widget* widget_;
		int row_;
		int column_;
		int rowSpan_;
		int columnSpan_;
		int stickySides_;
	};
	std::vector<CR42YGridChild> children_;

	CR42YGridChild* getChild(Gtk::Widget* child);

	void getRelativeCellSize(std::vector<CR42YGridRow>& rows, std::vector<double>& size, std::vector<bool>& ignore);
	void getMinimumCellEnds(std::vector<int>* columnEnds, std::vector<int>* rowEnds);

private:
	bool drawBorder_;
	bool drawBG_;
};

} /* namespace cr42y */

#endif /* SRC_CR42YGRID_H_ */
