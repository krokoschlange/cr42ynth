#include "CommunicatorMessage.h"

#include <memory.h>

namespace cr42y
{

CommunicatorMessage::CommunicatorMessage(const char* msg, size_t size,
		uint8_t* data, size_t dataSize, uint32_t id) :
				id_(id),
				buffer_(new uint8_t[sizeof(size_t) * 2 + size + dataSize]),
				size_(sizeof(size_t) * 2 + size + dataSize),
				offset_(0)
{
	memcpy(buffer_, &size, sizeof(size_t));
	memcpy(buffer_ + sizeof(size_t), msg, size);
	if (!data)
	{
		//ensure this
		dataSize = 0;
	}
	memcpy(buffer_ + sizeof(size_t) + size, &dataSize, sizeof(size_t));
	if (data && dataSize > 0)
	{
		
		memcpy(buffer_ + sizeof(size_t) * 2 + size, data, dataSize);
	}
}

CommunicatorMessage::~CommunicatorMessage()
{
	delete[] buffer_;
}

uint32_t CommunicatorMessage::id()
{
	return id_;
}

size_t CommunicatorMessage::read(uint8_t** out)
{
	*out = buffer_ + offset_;
	return size_ - offset_;
}

void CommunicatorMessage::offset(size_t offset)
{
	offset_ += offset;
	if (offset_ > size_ - 1)
	{
		offset_ = size_ - 1;
	}
}

} /* namespace cr42y */
