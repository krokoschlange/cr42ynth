#ifndef SRC_COMMON_OSCEVENTLISTENER_H_
#define SRC_COMMON_OSCEVENTLISTENER_H_

namespace cr42y
{
class OSCEvent;

class OSCEventListener
{
public:
	OSCEventListener();
	virtual ~OSCEventListener();

	virtual bool handleOSCEvent(OSCEvent* event) = 0;

	virtual void sendState() = 0;
};

} /* namespace cr42y */

#endif /* SRC_COMMON_OSCEVENTLISTENER_H_ */
