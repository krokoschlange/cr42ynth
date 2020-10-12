#include "CR42YWFView.h"

#include "CR42YButton.h"
#include "CR42YTheme.h"
#include "WavetableEditData.h"

namespace cr42y
{

CR42YWFView::CR42YWFView(CR42YUI* ui) :
		Glib::ObjectBase("CR42YWFView"),
		CR42YGrid(ui),
		showAll_(false),
		selectedWaveform_(0),
		data_(nullptr),
		editBtn_(new CR42YButton(ui))
{
	setDrawBorder(true);

	configureColumn(0, 10, 0, 0, 0, 0);
	configureColumn(1, 1, 0.003, 0.003, 0, 0);

	configureRow(0, 1, 0.003, 0.003, 0, 0);
	configureRow(1, 7, 0, 0, 0, 0);


	editBtn_->setText("Edit");
	put(editBtn_, 0, 1);
}

CR42YWFView::~CR42YWFView()
{
}

void CR42YWFView::setShowAll(bool showAll)
{
	showAll_ = showAll;
}

void CR42YWFView::setSelectedWaveform(int selectedWaveform)
{
	selectedWaveform_ = selectedWaveform;
}

void CR42YWFView::setData(WavetableEditData* data)
{
	data_ = data;
}

bool CR42YWFView::on_expose_event(GdkEventExpose* event)
{
	if (data_)
	{
		Glib::RefPtr<Gdk::Window> win = get_window();
		if (win)
		{
			Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context();

			if (showAll_)
			{
				//TODO: fake 3D view
			}
			else
			{
				int stepSize = data_->getWidth() / get_width();
				if (stepSize < 1)
				{
					stepSize = 1;
				}
				std::vector<float>* samples = data_->getSamples(selectedWaveform_, stepSize);

				float pixelsPerSample = (float) get_width() / samples->size();

				for (int i = 1; i < samples->size(); i++)
				{
					cr->move_to(i * pixelsPerSample, ((*samples)[i] * get_height() + get_height()) / 2);
					cr->line_to((i - 1) * pixelsPerSample, ((*samples)[i - 1] * get_height() + get_height()) / 2);
				}

				cr->move_to(get_width(), ((*samples)[0] * get_height() + get_height()) / 2);
				cr->line_to(get_width() - pixelsPerSample, ((*samples)[samples->size() - 1] * get_height() + get_height()) / 2);


				CR42YTheme* tm = theme();
				float* clr = tm->color(FG);

				cr->set_line_width(tm->lineThick());
				cr->set_source_rgba(clr[0], clr[1], clr[2], clr[3]);
				cr->stroke();

				delete samples;
			}
		}
	}

	return CR42YGrid::on_expose_event(event);
}


} /* namespace cr42y */
