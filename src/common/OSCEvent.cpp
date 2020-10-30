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
#include "OSCEvent.h"

#include <memory.h>

namespace cr42y
{

OSCEvent::OSCEvent(char* msg, int msgLength, void* data, int dataLength) :
		message_(nullptr),
		msgLength_(msgLength),
		data_(nullptr),
		dataLength_(dataLength)
{
	if (msg && msgLength > 0)
	{
		message_ = new char[msgLength];
		memcpy(message_, msg, msgLength);
	}
	if (data && dataLength > 0)
	{
		data_ = new char[dataLength];
		memcpy(data_, data, dataLength);
	}
}

OSCEvent::OSCEvent(const OSCEvent& other) :
		message_(nullptr),
		msgLength_(other.msgLength_),
		data_(nullptr),
		dataLength_(other.dataLength_)
{
	if (other.message_ && msgLength_ > 0)
	{
		message_ = new char[msgLength_];
		memcpy(message_, other.message_, msgLength_);
	}
	if (other.data_ && dataLength_ > 0)
	{
		data_ = new char[dataLength_];
		memcpy(data_, other.data_, dataLength_);
	}
}

OSCEvent::~OSCEvent()
{
	if (message_)
	{
		delete[] message_;
	}
	if (data_)
	{
		delete[] ((char*) data_);
	}
}

OSCEvent& OSCEvent::operator =(const OSCEvent& other)
{
	if (message_)
	{
		delete[] message_;
	}
	if (data_)
	{
		delete[] ((char*) data_);
	}

	msgLength_ = other.msgLength_;
	dataLength_ = other.dataLength_;

	if (other.message_ && msgLength_ > 0)
	{
		message_ = new char[msgLength_];
		memcpy(message_, other.message_, msgLength_);
	}
	if (other.data_ && dataLength_ > 0)
	{
		data_ = new char[dataLength_];
		memcpy(data_, other.data_, dataLength_);
	}
}

const char* OSCEvent::getMessage(int* len)
{
	if (len)
	{
		*len = msgLength_;
	}
	return message_;
}

void* OSCEvent::getData(int* len)
{
	if (len)
	{
		*len = dataLength_;
	}
	return data_;
}

} /* namespace cr42y */
