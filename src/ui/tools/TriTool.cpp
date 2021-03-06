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

#include "TriTool.h"
#include "WavetableEditData.h"
#include "WPFunction.h"

#include "common.h"

namespace cr42y
{

TriTool::TriTool(WavetableEditData* editData, int wtPos, float x, float y) :
		WTTool(editData, wtPos, x, y)
{
	part_ = new WPFunction(x, x + 0.0001, to_string(y));
	editData->addPart(wtPos, part_);
}

TriTool::~TriTool()
{
	
}

void TriTool::motion(float x, float y)
{
	if (x != startX_)
	{
		float m = (y - startY_) / (x - startX_);
		float c = - x * m + y;
		((WPFunction*) part_)->setFunction(to_string(m) + "*x+" + to_string(c));
		if (x < startX_)
		{
			part_->setStart(x);
			part_->setEnd(startX_);
		}
		else
		{
			part_->setStart(startX_);
			part_->setEnd(x);
		}
	}
}


} /* namespace cr42y */
