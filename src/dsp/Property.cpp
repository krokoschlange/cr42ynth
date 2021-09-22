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


#include "Property.h"
#include "CR42YnthDSP.h"

namespace cr42y
{

Property::Property(Voice* vce, Control* ctrl, float val, float min, float max, Generator* gen) :
		voice(vce),
		value(val),
		minimum(min),
		maximum(max),
		generator(gen)
{
	if (ctrl)
	{
		listenTo(ctrl);
	}
}

Property::~Property()
{
}

void Property::valueCallback(float val, Control*)
{
	if (!generator)
	{
		value = val;
	}
}

void Property::minCallback(float min, Control*)
{
	minimum = min;
}

void Property::maxCallback(float max, Control*)
{
	maximum = max;
}

void Property::genCallback(uint32_t gen, Control*)
{
	if (generator)
	{
		generator->removeProperty(this);
	}
	generator = CR42YnthDSP::getInstance()->getGeneratorFromString(gen);

	if (generator)
	{
		generator->addProperty(this);
	}
}

void Property::listenTo(Control* ctrl)
{
	ControlListener::connect(*ctrl);
	genCallback(ctrl->getGenerator(), ctrl);
	valueCallback(ctrl->getValue(), ctrl);
	minCallback(ctrl->getMin(), ctrl);
	maxCallback(ctrl->getMax(), ctrl);
}

Voice* Property::getVoice()
{
	return voice;
}

Generator* Property::getGenerator()
{
	return generator;
}

} /* namespace cr42y */
