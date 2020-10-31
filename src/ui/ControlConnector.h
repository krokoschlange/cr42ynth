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
#ifndef SRC_UI_CONTROLCONNECTOR_H_
#define SRC_UI_CONTROLCONNECTOR_H_

#include "ControlListener.h"

#include <functional>

namespace cr42y
{

class ControlConnector : public ControlListener
{
public:
	ControlConnector();
	virtual ~ControlConnector();

	void setControlValue(double value);
	void setControlMin(double min);
	void setControlMax(double max);
	void setControlGenerator(std::string generator);

	void setWidgetValueSetter(std::function<void(double)> widgetSetValue);
	void setWidgetMinSetter(std::function<void(double)> widgetSetMin);
	void setWidgetMaxSetter(std::function<void(double)> widgetSetMax);
	void setWidgetGeneratorSetter(std::function<void(std::string)> widgetSetGenerator);

	void valueCallback(float val);
	void minCallback(float min);
	void maxCallback(float max);
	void genCallback(std::string gen);

private:
	std::function<void(double)> widgetSetValue_;
	std::function<void(double)> widgetSetMin_;
	std::function<void(double)> widgetSetMax_;
	std::function<void(std::string)> widgetSetGenerator_;
};

} /* namespace cr42y */

#endif /* SRC_UI_CONTROLCONNECTOR_H_ */
