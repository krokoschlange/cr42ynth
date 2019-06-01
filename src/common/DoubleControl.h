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

#ifndef SRC_COMMON_DOUBLECONTROL_H_
#define SRC_COMMON_DOUBLECONTROL_H_

#include <lv2/atom/atom.h>

#include "DefinitionHandler.h"
#include "PortCommunication/MessageReceiver.h"

namespace cr42y
{
class MessageReceiver;

class DoubleControl : public MessageReceiver
{
public:
	DoubleControl(int msgType, PortCommunicator* comm = nullptr, double val = 0,
			double mi = 0, double ma = 1, LV2_URID dKey =
					DefinitionHandler::getInstance()->msg_key);
	virtual ~DoubleControl();

	virtual void receive(LV2_Atom_Object* data);

	double getValue();
	double getMin();
	double getMax();

	void setValue(double val);
	void setMin(double m);
	void setMax(double m);

private:
	double value;
	double min;
	double max;

	LV2_URID dataKey;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_DOUBLECONTROL_H_ */
