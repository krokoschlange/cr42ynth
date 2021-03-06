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


#ifndef SRC_UI_CR42YTOGGLESELECTOR_H_
#define SRC_UI_CR42YTOGGLESELECTOR_H_

#include "CR42YRelativeContainer.h"

namespace cr42y
{

class CR42YToggle;

class CR42YToggleSelector : public CR42YRelativeContainer
{
public:
	CR42YToggleSelector(CR42YUI* ui);
	virtual ~CR42YToggleSelector();

	void select(int selected, bool callback = true);
	int selected();

	void putToggle(CR42YToggle* toggle, double x, double y, double w, double h,
			double padLeft = 0, double padTop = 0, double padRight = 0,
			double padBottom = 0);

	void removeToggle(CR42YToggle* toggle);

	sigc::signal<void, int> signalSelected();

private:
	std::vector<std::pair<CR42YToggle*, sigc::connection>> toggles_;
	int selected_;
	sigc::signal<void, int> signalSelected_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CR42YTOGGLESELECTOR_H_ */
