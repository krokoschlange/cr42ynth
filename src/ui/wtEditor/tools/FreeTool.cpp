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

#include <iostream>

#include "FreeTool.h"
#include "WavetableEditData.h"
#include "WPSamples.h"

namespace cr42y
{

FreeTool::FreeTool(WavetableEditData* eData, int wtPos, float x, float y) :
		WTTool(eData, wtPos, x, y),
		lastX(x),
		lastY(y)
{
	std::vector<float> v;
	v.push_back(y);
	part = new WPSamples(x, x + 0.0001, v);
	eData->addPart(wtPos, part);
}

FreeTool::~FreeTool()
{
}

void FreeTool::motion(float x, float y)
{
	int startPos = lastX * editData->getWidth();
	int endPos = x * editData->getWidth();
	
	bool switched = false;
	
	if (startPos > endPos)
	{
		switched = true;
		int tmp = startPos;
		startPos = endPos;
		endPos = tmp;
	}
	
	std::vector<float>* samples = ((WPSamples*) part)->getSamples();
	int vStart = part->getStart() * editData->getWidth();
	int vEnd = vStart + samples->size();
	
	if (startPos < vStart)
	{
		float arr[vStart - startPos];
		samples->insert(samples->begin(), arr, arr + (vStart - startPos));
		part->setStart(x < lastX ? x : lastX);
		vStart = startPos;
	}
	if (endPos > vEnd)
	{
		float arr[endPos - vEnd];
		samples->insert(samples->end(), arr, arr + (endPos - vEnd));
		if (part->getEnd() < (x > lastX ? x : lastX))
		{
			part->setEnd(x > lastX ? x : lastX);
		}
		vEnd = endPos;
	}
	
	int relStartPos = startPos - vStart;
	int relEndPos = relStartPos + (endPos - startPos);
	
	if (switched)
	{
		for (int i = relStartPos; i < relEndPos; i++)
		{
			(*samples)[i] = y + (float) (i - relStartPos) / (relStartPos - relEndPos) * (y - lastY);
		}
	}
	else
	{
		for (int i = relStartPos; i < relEndPos; i++)
		{
			(*samples)[i] = lastY + (float) (i - relStartPos) / (relEndPos - relStartPos) * (y - lastY);
		}
	}
	
	lastX = x;
	lastY = y;
}


} /* namespace cr42y */
