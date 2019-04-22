#ifndef CR42YNTH_UI
#define CR42YNTH_UI

#include "avtk.hxx"
#include "customlist.h"

namespace Avtk
{
	class Widget;
}

namespace cr42y
{

class Cr42ynthUI : public Avtk::UI
{
public:
	Cr42ynthUI(PuglNativeWindow parent = 0);
	virtual ~Cr42ynthUI() {}
	
	virtual void widgetValueCB(Avtk::Widget * widget);
	virtual void lv2PortEvent(
		uint32_t index,
		uint32_t buffer_size,
		uint32_t format,
		const void* buffer
	);
	
private:
	CustomList *list;
	
};
}

#endif
