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
#include "CR42YGrid.h"
#include "helpers.h"
#include "CR42YTheme.h"

namespace cr42y
{

CR42YGrid::CR42YGrid(CR42YUI* ui) :
		Glib::ObjectBase("CR42YGrid"),
		Gtk::Container(),
		CR42YWidget(ui),
		drawBorder_(false),
		drawBG_(false)
{
	set_flags(Gtk::NO_WINDOW);
	configureColumn(0, 1, 0, 0, 0, 0);
	configureRow(0, 1, 0, 0, 0, 0);
}

CR42YGrid::~CR42YGrid()
{
}

void CR42YGrid::addColumn(double weight, double padFront, double padBack,
		int minSize, int maxSize)
{
	CR42YGridRow column;
	column.weight_ = weight;
	column.padFront_ = padFront;
	column.padBack_ = padBack;
	column.minSize_ = minSize;
	column.maxSize_ = maxSize;
	column.allocated_ = 0;
	columns_.push_back(column);
}

void CR42YGrid::addRow(double weight, double padFront, double padBack,
		int minSize, int maxSize)
{
	CR42YGridRow row;
	row.weight_ = weight;
	row.padFront_ = padFront;
	row.padBack_ = padBack;
	row.minSize_ = minSize;
	row.maxSize_ = maxSize;
	row.allocated_ = 0;
	rows_.push_back(row);
}

void CR42YGrid::removeRow(int row)
{
	if (0 <= row && (size_t) row < rows_.size())
	{
		rows_.erase(rows_.begin() + row);
	}
}

void CR42YGrid::removeColumn(int column)
{
	if (0 <= column && (size_t) column < columns_.size())
	{
		columns_.erase(columns_.begin() + column);
	}
}

void CR42YGrid::configureColumn(int column, double weight, double padFront,
		double padBack, int minSize, int maxSize)
{
	while ((size_t) column >= columns_.size())
	{
		addColumn(0, 0, 0, 0, 0);
	}
	columns_[column].weight_ = weight;
	columns_[column].padFront_ = padFront;
	columns_[column].padBack_ = padBack;
	columns_[column].minSize_ = minSize;
	columns_[column].maxSize_ = maxSize;
}

void CR42YGrid::configureRow(int row, double weight, double padFront,
		double padBack, int minSize, int maxSize)
{
	while ((size_t) row >= rows_.size())
	{
		addRow(0, 0, 0, 0, 0);
	}
	rows_[row].weight_ = weight;
	rows_[row].padFront_ = padFront;
	rows_[row].padBack_ = padBack;
	rows_[row].minSize_ = minSize;
	rows_[row].maxSize_ = maxSize;
}

void CR42YGrid::put(Gtk::Widget* widget, int row, int column, int rowSpan,
		int columnSpan, int stickySides)
{
	CR42YGridChild childData;
	childData.widget_ = widget;
	childData.row_ = row;
	childData.column_ = column;
	childData.rowSpan_ = rowSpan;
	childData.columnSpan_ = columnSpan;
	childData.stickySides_ = stickySides;

	children_.push_back(childData);
	widget->set_parent(*this);
}

void CR42YGrid::on_remove(Gtk::Widget* child)
{
	CR42YGridChild* childData = getChild(child);
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (childData == &(children_[i]))
		{
			children_.erase(children_.begin() + i);
			i--;
		}
	}
	child->unparent();
}

void CR42YGrid::on_add(Gtk::Widget* child)
{
	put(child, 0, 0);
}

void CR42YGrid::setDrawBorder(bool drawBorder)
{
	drawBorder_ = drawBorder;
}

void CR42YGrid::setDrawBG(bool drawBG)
{
	drawBG_ = drawBG;
}

void CR42YGrid::on_realize()
{
	Gtk::Container::on_realize();
	set_realized(true);
}

bool CR42YGrid::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();
		CR42YTheme* tm = theme();
		float* clr = tm->color(BG_DARK);
		if (drawBG_)
		{
			cr->rectangle(event->area.x, event->area.y, event->area.width,
					event->area.height);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->fill();
		}

		if (drawBorder_)
		{
			clr = tm->color(FG);
			cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
			cr->set_line_width(tm->lineThick());

			if (has_no_window())
			{
				cr42y_rounded_rectangle(cr, get_allocation().get_x(),
						get_allocation().get_y(), get_width(), get_height(),
						tm->cornerRadius(), tm->lineThick());
			}
			else
			{
				cr42y_rounded_rectangle(cr, 0, 0, get_width(), get_height(),
						tm->cornerRadius(), tm->lineThick());
			}
			cr->stroke();
		}
	}
	return Gtk::Container::on_expose_event(event);
}

void CR42YGrid::on_size_allocate(Gtk::Allocation& alloc)
{
	set_allocation(alloc);

	if (!has_no_window())
	{
		get_window()->move_resize(alloc.get_x(), alloc.get_y(),
				alloc.get_width(), alloc.get_height());
	}

	std::vector<int> columnEnds(columns_.size(), 0);
	std::vector<int> rowEnds(rows_.size(), 0);

	//getMinimumCellEnds(&columnEnds, &rowEnds);
	struct Alloc
	{
		size_t idx_;
		int alloc_;
		double param_;
	};

	std::vector<Alloc> columnAllocs(columns_.size());

	if (columns_.size() > 1)
	{
		std::vector<int> columnMinWidths(columns_.size(), 0);
		columnMinWidths[0] = columnEnds[0];
		for (size_t i = 1; i < columnEnds.size(); i++)
		{
			columnMinWidths[i] = columnEnds[i] - columnEnds[i - 1];
		}

		int totalWidth = alloc.get_width();

		size_t maxParam = 0;
		size_t max2Param = 0;
		for (size_t i = 0; i < columns_.size(); i++)
		{
			columnAllocs[i] =
			{	i, columnMinWidths[i], columns_[i].weight_ / (columnMinWidths[i] + 1)};
			totalWidth -= columnMinWidths[i];

			if (columnAllocs[i].param_ > columnAllocs[maxParam].param_)
			{
				maxParam = i;
			}
		}
		max2Param = maxParam == 0 ? 1 : 0;
		for (size_t i = 0; i < columnAllocs.size(); i++)
		{
			if (columnAllocs[i].param_ > columnAllocs[max2Param].param_
				&& i != maxParam)
			{
				max2Param = i;
			}
		}

		while (totalWidth > 0)
		{
			Alloc* alloc = &(columnAllocs[maxParam]);
			alloc->alloc_++;
			totalWidth--;
			alloc->param_ = columns_[alloc->idx_].weight_ / (alloc->alloc_ + 1);

			if (columnAllocs[max2Param].param_ > columnAllocs[maxParam].param_
					|| (columns_[alloc->idx_].maxSize_ > 0
							&& alloc->alloc_ >= columns_[alloc->idx_].maxSize_))
			{
				maxParam = max2Param;
				double maxParamVal = 0;
				max2Param = 0;
				for (size_t i = 0; i < columnAllocs.size(); i++)
				{
					if (columnAllocs[i].param_ > maxParamVal && i != maxParam
							&& !(columns_[columnAllocs[i].idx_].maxSize_ > 0
									&& columnAllocs[i].alloc_
											>= columns_[columnAllocs[i].idx_].maxSize_))
					{
						maxParamVal = columnAllocs[i].param_;
						max2Param = i;
					}
				}
			}
		}

		/*double totalColumnWeight = 0;
		 for (int i = 0; i < columns_.size(); i++)
		 {
		 totalColumnWeight += columns_[i].weight_;
		 }
		 for (int i = 0; i < columns_.size(); i++)
		 {
		 columnAllocs[i] =
		 {	i, (int) (totalWidth * (columns_[i].weight_ / totalColumnWeight)), 0};
		 }*/
	}
	else
	{
		columnAllocs[0].alloc_ = alloc.get_width();
	}

	std::vector<Alloc> rowAllocs(rows_.size());

	if (rows_.size() > 1)
	{
		std::vector<int> rowMinHeights(rows_.size(), 0);
		rowMinHeights[0] = rowEnds[0];
		for (size_t i = 1; i < rowEnds.size(); i++)
		{
			rowMinHeights[i] = rowEnds[i] - rowEnds[i - 1];
		}

		int totalHeight = alloc.get_height();

		size_t maxParam = 0;
		size_t max2Param = 0;
		for (size_t i = 0; i < rows_.size(); i++)
		{
			rowAllocs[i] =
			{	i, rowMinHeights[i], rows_[i].weight_};
			totalHeight -= rowMinHeights[i];

			if (rowAllocs[i].param_ > rowAllocs[maxParam].param_)
			{
				maxParam = i;
			}
		}
		max2Param = maxParam == 0 ? 1 : 0;
		for (size_t i = 0; i < rowAllocs.size(); i++)
		{
			if (rowAllocs[i].param_ > rowAllocs[max2Param].param_
					&& i != maxParam)
			{
				max2Param = i;
			}
		}

		while (totalHeight > 0)
		{
			Alloc* alloc = &(rowAllocs[maxParam]);
			alloc->alloc_++;
			totalHeight--;
			alloc->param_ = rows_[alloc->idx_].weight_ / (alloc->alloc_ + 1);

			if (rowAllocs[max2Param].param_ > rowAllocs[maxParam].param_
					|| (rows_[alloc->idx_].maxSize_ > 0
							&& alloc->alloc_ >= rows_[alloc->idx_].maxSize_))
			{
				maxParam = max2Param;
				double maxParamVal = 0;
				max2Param = 0;
				for (size_t i = 0; i < rowAllocs.size(); i++)
				{
					if (rowAllocs[i].param_ > maxParamVal && i != maxParam
							&& !(rows_[rowAllocs[i].idx_].maxSize_ > 0
									&& rowAllocs[i].alloc_
											>= rows_[rowAllocs[i].idx_].maxSize_))
					{
						maxParamVal = rowAllocs[i].param_;
						max2Param = i;
					}
				}
			}
		}

		/*double totalRowWeight = 0;
		 for (int i = 0; i < rows_.size(); i++)
		 {
		 totalRowWeight += rows_[i].weight_;
		 }
		 for (int i = 0; i < rows_.size(); i++)
		 {
		 rowAllocs[i] =
		 {	i, (int) (totalHeight * (rows_[i].weight_ / totalRowWeight)), 0};
		 }*/
	}
	else
	{
		rowAllocs[0].alloc_ = alloc.get_height();
	}

	//std::vector<int> columnWidths(columns_.size(), 0);
	for (size_t i = 0; i < columnAllocs.size(); i++)
	{
		columns_[i].allocated_ = columnAllocs[i].alloc_;
	}
	//std::vector<int> rowHeights(rows_.size(), 0);
	for (size_t i = 0; i < rowAllocs.size(); i++)
	{
		rows_[i].allocated_ = rowAllocs[i].alloc_;
	}

	Gtk::Allocation childAlloc;
	for (size_t i = 0; i < children_.size(); i++)
	{
		CR42YGridChild child = children_[i];

		int cellX = 0, cellY = 0, cellW = 0, cellH = 0;

		for (int j = 0; j < child.column_; j++)
		{
			cellX += columns_[j].allocated_;
		}
		for (int j = child.column_; j < child.column_ + child.columnSpan_; j++)
		{
			cellW += columns_[j].allocated_;
		}
		for (int j = 0; j < child.row_; j++)
		{
			cellY += rows_[j].allocated_;
		}
		for (int j = child.row_; j < child.row_ + child.rowSpan_; j++)
		{
			cellH += rows_[j].allocated_;
		}

		double padLeft = columns_[child.column_].padFront_;
		if (padLeft < 1)
		{
			padLeft *= get_toplevel()->get_height();
		}
		double padRight = columns_[child.column_ + child.columnSpan_ - 1].padBack_;
		if (padRight < 1)
		{
			padRight *= get_toplevel()->get_height();
		}
		double padTop = rows_[child.row_].padFront_;
		if (padTop < 1)
		{
			padTop *= get_toplevel()->get_height();
		}
		double padBottom = rows_[child.row_ + child.rowSpan_ - 1].padBack_;
		if (padBottom < 1)
		{
			padBottom *= get_toplevel()->get_height();
		}

		Gtk::Requisition childReq = child.widget_->size_request();
		int preferredW = childReq.width + padLeft + padRight;
		int preferredH = childReq.height + padTop + padBottom;

		if (preferredW > cellW)
		{
			preferredW = cellW;
		}
		if (preferredH > cellH)
		{
			preferredH = cellH;
		}

		int x = 0, y = 0, w = 0, h = 0;
		int x2 = 0, y2 = 0;

		if ((child.stickySides_ & W) > 0)
		{
			x = cellX;
		}
		else if ((child.stickySides_ & E) > 0)
		{
			x = cellX + cellW - preferredW;
		}
		else
		{
			x = cellX + (cellW - preferredW) / 2;
		}
		if ((child.stickySides_ & E) > 0)
		{
			x2 = cellX + cellW;
		}
		else if ((child.stickySides_ & W) > 0)
		{
			x2 = cellX + preferredW;
		}
		else
		{
			x2 = cellX + (cellW + preferredW) / 2;
		}

		if ((child.stickySides_ & N) > 0)
		{
			y = cellY;
		}
		else if ((child.stickySides_ & S) > 0)
		{
			y = cellY + cellH - preferredH;
		}
		else
		{
			y = cellY + (cellY - preferredH) / 2;
		}
		if ((child.stickySides_ & S) > 0)
		{
			y2 = cellY + cellH;
		}
		else if ((child.stickySides_ & N) > 0)
		{
			y2 = cellY + preferredH;
		}
		else
		{
			y2 = cellY + (cellH + preferredH) / 2;
		}

		w = x2 - x;
		h = y2 - y;

		x += padLeft;
		w -= padLeft + padRight;
		y += padTop;
		h -= padTop + padBottom;

		if (has_no_window())
		{
			childAlloc.set_x(get_allocation().get_x());
			childAlloc.set_y(get_allocation().get_y());
		}
		childAlloc.set_x(childAlloc.get_x() + x);
		childAlloc.set_y(childAlloc.get_y() + y);
		childAlloc.set_width(w);
		childAlloc.set_height(h);

		child.widget_->size_allocate(childAlloc);
	}
}

void CR42YGrid::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();

	std::vector<int> columnEnds;
	std::vector<int> rowEnds;

	getMinimumCellEnds(&columnEnds, &rowEnds);

	requisition->width = columnEnds[columnEnds.size() - 1];
	requisition->height = rowEnds[rowEnds.size() - 1];
}

void CR42YGrid::forall_vfunc(gboolean, GtkCallback callback,
		gpointer callback_data)
{
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (children_[i].widget_->gobj())
		{
			callback(children_[i].widget_->gobj(), callback_data);
		}
	}
}

void CR42YGrid::getRelativeCellSize(std::vector<CR42YGridRow>& rows,
		std::vector<double>& size, std::vector<bool>& ignore)
{
	int totalWeight = 0;
	for (size_t i = 0; i < rows.size(); i++)
	{
		if (!ignore[i])
		{
			totalWeight += rows[i].weight_;
		}
	}

	if (totalWeight > 0)
	{
		for (size_t i = 0; i < rows.size(); i++)
		{
			if (!ignore[i])
			{
				size[i] = rows[i].weight_ / totalWeight;
			}
		}
	}
}

void CR42YGrid::getMinimumCellEnds(std::vector<int>* columnEnds,
		std::vector<int>* rowEnds)
{
	*columnEnds = std::vector<int>(columns_.size(), 0);
	for (size_t column = 0; column < columns_.size(); column++)
	{
		int previousEnd = 0;
		if (column > 0)
		{
			previousEnd = (*columnEnds)[column - 1];
		}
		(*columnEnds)[column] = previousEnd + columns_[column].minSize_;

		for (size_t i = 0; i < children_.size(); i++)
		{
			if ((size_t) (children_[i].column_ + children_[i].columnSpan_) - 1 == column)
			{
				int startEnd = 0;
				if (children_[i].column_ > 0)
				{
					startEnd = (*columnEnds)[children_[i].column_ - 1];
				}

				int width, height;
				children_[i].widget_->get_size_request(width, height);

				if (width < 0)
				{
					Gtk::Requisition req = children_[i].widget_->size_request();
					width = req.width;
				}
				if (columns_[children_[i].column_].padFront_ >= 1)
				{
					width += columns_[children_[i].column_].padFront_;
				}
				else
				{
					width += std::round(
							columns_[children_[i].column_].padFront_
									* get_toplevel()->get_height());
				}
				if (columns_[children_[i].column_ + children_[i].columnSpan_ - 1].padBack_
						>= 1)
				{
					width += columns_[children_[i].column_
							+ children_[i].columnSpan_ - 1].padBack_;
				}
				else
				{
					width += std::round(
							columns_[children_[i].column_
									+ children_[i].columnSpan_ - 1].padBack_
									* get_toplevel()->get_height());
				}
				int columnEnd = startEnd + width;

				if (columns_[column].maxSize_ > 0
						&& columnEnd - previousEnd > columns_[column].maxSize_)
				{
					columnEnd = previousEnd + columns_[column].maxSize_;
				}

				if (columnEnd > (*columnEnds)[column])
				{
					(*columnEnds)[column] = columnEnd;
				}
			}
		}
	}
	*rowEnds = std::vector<int>(rows_.size(), 0);
	for (size_t row = 0; row < rows_.size(); row++)
	{
		int previousEnd = 0;
		if (row > 0)
		{
			previousEnd = (*rowEnds)[row - 1];
		}
		(*rowEnds)[row] = previousEnd + rows_[row].minSize_;

		for (size_t i = 0; i < children_.size(); i++)
		{
			if ((size_t) (children_[i].row_ + children_[i].rowSpan_) - 1 == row)
			{
				int startEnd = 0;
				if (children_[i].row_ > 0)
				{
					startEnd = (*rowEnds)[children_[i].row_ - 1];
				}

				int width, height;
				children_[i].widget_->get_size_request(width, height);
				if (height < 0)
				{
					Gtk::Requisition req = children_[i].widget_->size_request();
					height = req.height;
				}
				if (rows_[children_[i].row_].padFront_ >= 1)
				{
					height += rows_[children_[i].row_].padFront_;
				}
				else
				{
					height += std::round(
							rows_[children_[i].row_].padFront_
									* get_toplevel()->get_height());
				}
				if (rows_[children_[i].row_ + children_[i].rowSpan_ - 1].padBack_
						>= 1)
				{
					height += rows_[children_[i].row_ + children_[i].rowSpan_
							- 1].padBack_;
				}
				else
				{
					height += std::round(
							rows_[children_[i].row_ + children_[i].rowSpan_ - 1].padBack_
									* get_toplevel()->get_height());
				}

				//height += rows_[children_[i].row_].padFront_ + rows_[children_[i].row_ + children_[i].rowSpan_ - 1].padBack_;
				int rowEnd = startEnd + height;

				if (rows_[row].maxSize_ > 0
						&& rowEnd - previousEnd > rows_[row].maxSize_)
				{
					rowEnd = previousEnd + rows_[row].maxSize_;
				}

				if (rowEnd > (*rowEnds)[row])
				{
					(*rowEnds)[row] = rowEnd;
				}
			}
		}
	}
}

CR42YGrid::CR42YGridChild* CR42YGrid::getChild(Gtk::Widget* child)
{
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (children_[i].widget_ == child)
		{
			return &(children_[i]);
		}
	}
	return nullptr;
}

} /* namespace cr42y */
