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

#include "CR42YWaveformEditorControlPanel.h"
#include "CR42YLabel.h"
#include "CR42YButton.h"
#include "CR42YIntegerEditor.h"
#include "CR42YWaveformEditor.h"
#include "WavetableEditController.h"
#include "CR42YUI.h"

namespace cr42y
{

CR42YWaveformEditorControlPanel::CR42YWaveformEditorControlPanel(CR42YUI* ui,
		CR42YWaveformEditor* wfEditor, WavetableEditController* controller) :
		Glib::ObjectBase("CR42YWaveformEditorControlPanel"),
		wfEditor_(wfEditor),
		controller_(controller),
		CR42YRelativeContainer(ui),
		gridXLabel_(new CR42YLabel(ui)),
		gridXEditor_(new CR42YIntegerEditor(ui)),
		gridYLabel_(new CR42YLabel(ui)),
		gridYEditor_(new CR42YIntegerEditor(ui)),
		toSinBtn_(new CR42YButton(ui)),
		toSinHQBtn_(new CR42YButton(ui)),
		normalizeBtn_(new CR42YButton(ui)),
		nextBtn_(new CR42YButton(ui)),
		prevBtn_(new CR42YButton(ui)),
		deleteBtn_(new CR42YButton(ui))
{
	setDrawBorder(true);

	gridXLabel_->setText("Grid X");
	gridXEditor_->setMin(0);
	gridXEditor_->setMax(0, false);
	gridYLabel_->setText("Grid Y");
	gridYEditor_->setMin(0);
	gridYEditor_->setMax(0, false);

	toSinBtn_->setText("TO SIN");
	toSinHQBtn_->setText("TO SIN (HQ)");
	normalizeBtn_->setText("NORMALIZE");

	Cairo::RefPtr<Cairo::ImageSurface> right = Cairo::ImageSurface::create_from_png(
			ui->resourceRoot() + "media/right.png");
	nextBtn_->setSurfActive(right);
	nextBtn_->setSurfInactive(right);
	Cairo::RefPtr<Cairo::ImageSurface> left = Cairo::ImageSurface::create_from_png(
			ui->resourceRoot() + "media/left.png");
	prevBtn_->setSurfActive(left);
	prevBtn_->setSurfInactive(left);

	deleteBtn_->setText("DELETE");

	gridXEditor_->signalChanged().connect(
			sigc::mem_fun(this,
					&CR42YWaveformEditorControlPanel::gridXCallback));
	gridYEditor_->signalChanged().connect(
			sigc::mem_fun(this,
					&CR42YWaveformEditorControlPanel::gridYCallback));
	toSinBtn_->signalClicked().connect(
			sigc::bind<bool>(
					sigc::mem_fun(this,
							&CR42YWaveformEditorControlPanel::toSinCallback),
					false));
	toSinHQBtn_->signalClicked().connect(
			sigc::bind<bool>(
					sigc::mem_fun(this,
							&CR42YWaveformEditorControlPanel::toSinCallback),
					true));
	normalizeBtn_->signalClicked().connect(
			sigc::mem_fun(this,
					&CR42YWaveformEditorControlPanel::normalizeCallback));
	nextBtn_->signalClicked().connect(
			sigc::mem_fun(this,
					&CR42YWaveformEditorControlPanel::nextCallback));
	prevBtn_->signalClicked().connect(
			sigc::mem_fun(this,
					&CR42YWaveformEditorControlPanel::prevCallback));
	deleteBtn_->signalClicked().connect(
			sigc::mem_fun(this,
					&CR42YWaveformEditorControlPanel::deleteCallback));

	put(gridXLabel_, 0, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
	put(gridXEditor_, 0.1, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
	put(gridYLabel_, 0.2, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
	put(gridYEditor_, 0.3, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
	put(toSinBtn_, 0.4, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
	put(toSinHQBtn_, 0.5, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
	put(normalizeBtn_, 0.6, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);

	put(prevBtn_, 0.8, 0, 0.05, 1, 0.003, 0.003, 0.003, 0.003);
	put(nextBtn_, 0.85, 0, 0.05, 1, 0.003, 0.003, 0.003, 0.003);
	put(deleteBtn_, 0.9, 0, 0.1, 1, 0.003, 0.003, 0.003, 0.003);
}

CR42YWaveformEditorControlPanel::~CR42YWaveformEditorControlPanel()
{
	std::vector<Gtk::Widget*> children = get_children();

	for (int i = 0; i < children.size(); i++)
	{
		remove(*(children[i]));
		delete children[i];
	}
}

void CR42YWaveformEditorControlPanel::gridXCallback(int val)
{
	controller_->setGridX(val);
	wfEditor_->queue_draw();
}

void CR42YWaveformEditorControlPanel::gridYCallback(int val)
{
	controller_->setGridY(val);
	wfEditor_->queue_draw();
}

void CR42YWaveformEditorControlPanel::toSinCallback(bool highQuality)
{
	controller_->convertToSin(highQuality);
}

void CR42YWaveformEditorControlPanel::normalizeCallback()
{
	controller_->normalizeHarmonic();
}

void CR42YWaveformEditorControlPanel::nextCallback()
{
	controller_->selectPart(controller_->getSelectedPart() + 1);
}

void CR42YWaveformEditorControlPanel::prevCallback()
{
	if (controller_->getSelectedPart() > 0)
	{
		controller_->selectPart(controller_->getSelectedPart() - 1);
	}
}

void CR42YWaveformEditorControlPanel::deleteCallback()
{
	controller_->removePart(controller_->getSelectedPart());
}

} /* namespace cr42y */
