#ifndef SRC_COMMON_LV2_COMMUNICATORMESSAGE_H_
#define SRC_COMMON_LV2_COMMUNICATORMESSAGE_H_

#include <stddef.h>
#include <stdint.h>

namespace cr42y
{

class CommunicatorMessage
{
public:
	CommunicatorMessage(const char* msg, size_t size,
			uint8_t* data, size_t dataSize, uint32_t id);
	virtual ~CommunicatorMessage();

	uint32_t id();

	size_t read(uint8_t** out);

	void offset(size_t offset);

private:
	uint32_t id_;

	uint8_t* buffer_;
	size_t size_;
	size_t offset_;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_LV2_COMMUNICATORMESSAGE_H_ */
