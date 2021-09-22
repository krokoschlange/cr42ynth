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


#include "CR42YAutomationSelector.h"

#include "AutomationEditController.h"
#include "CR42YAutomationItem.h"

namespace cr42y
{

CR42YAutomationSelector::CR42YAutomationSelector(CR42YUI* ui, AutomationEditController* controller) :
		CR42YToggleSelector(ui),
		controller_(controller),
		boxSize_(50)
{
	update();
}

CR42YAutomationSelector::~CR42YAutomationSelector()
{
}

void CR42YAutomationSelector::on_size_request(Gtk::Requisition* requisition)
{
	*requisition = Gtk::Requisition();
	if (controller_)
	{
		int height = boxSize_ * controller_->getAutomationCount();
		requisition->height = height; //std::max<int>(height, get_parent()->get_height());
		requisition->width = 10;
	}
	else
	{
		requisition->height = 10; //get_parent()->get_height();
		requisition->width = 10;
	}
}

void CR42YAutomationSelector::update()
{
	if (controller_)
	{		
		for (size_t i = items_.size(); i < controller_->getAutomationCount(); i++)
		{
			CR42YAutomationItem* item = new CR42YAutomationItem(ui_);
			putToggle(item, 0, i * boxSize_, 1, boxSize_);
			items_.push_back(item);
			item->show();
		}
		while ((size_t) controller_->getAutomationCount() < items_.size())
		{
			removeToggle(items_[items_.size() - 1]);
			delete items_[items_.size() - 1];
			items_.erase(items_.begin() + items_.size() - 1);
		}
		for (size_t i = 0; i < items_.size(); i++)
		{
			/*std::string typeText = "LFO";
			if (controller_->getType(i) > 0)
			{
				typeText = "ENV";
			}
			items_[i]->setText(std::to_string(controller_->getID(i)) + " " + typeText);*/
			uint8_t type = TYPE_ENV;
			if (controller_->getType(i) > 0)
			{
				type = TYPE_LFO;
			}
			uint32_t id = controller_->getID(i);
			uint32_t gen = (((uint32_t) type) << 24) | (id & 0xffffff);
			items_[i]->setGenerator(gen);
		}
		select(controller_->selectedAutomation());
	}
	set_size_request(10, boxSize_ * items_.size());
}

}
