/*******************************************************************************
 * Copyright (c) 2019 krokoschlange and contributors.
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

#include "SinSlopeTool.h"

#include "WPFunction.h"
#include "WaveformPart.h"
#include "WavetableEditData.h"

#include "common.h"

namespace cr42y
{

SinSlopeTool::SinSlopeTool(WavetableEditData* eData, int wtPos, float x, float y) :
		WTTool(eData, wtPos, x, y)
{
	part = new WPFunction(x, x + 0.0001, to_string(y));
	eData->addPart(wtPos, part);
}

SinSlopeTool::~SinSlopeTool()
{
	
}

void SinSlopeTool::motion(float x, float y)
{
	if (x != startX)
		{
		// a*cos(2pi*(b(x-d)))+c
		float a = (startY - y) / 2;
		float b = 0.5 / (startX - x);
		float c = (startY + y) / 2;
		float d = startX;
		std::string func = to_string(a) + "*cos(2pi*" + to_string(b) + "(x-" + to_string(d) + "))+" + to_string(c);
		((WPFunction*) part)->setFunction(func);
		if (x < startX)
		{
			part->setStart(x);
			part->setEnd(startX);
		}
		else
		{
			part->setStart(startX);
			part->setEnd(x);
		}
	}
}


} /* namespace cr42y */
