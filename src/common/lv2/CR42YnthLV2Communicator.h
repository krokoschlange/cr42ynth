#ifndef CR42YNTHLV2COMMUNICATOR_H
#define CR42YNTHLV2COMMNICATOR_H

#include "CR42YnthCommunicator.h"

#include <lv2/atom/forge.h>

#include <queue>



namespace cr42y
{
class CommunicatorMessage;

class CR42YnthLV2Communicator : public CR42YnthCommunicator
{
public:
	CR42YnthLV2Communicator();
	virtual ~CR42YnthLV2Communicator();

	void initURIDMap(LV2_URID_Map* map);

	size_t getRequiredBufferSize(size_t dataSize);



	void prepareAtomWrite();
	size_t writeMsgAtom(uint8_t* data, size_t size);

	void queueMsg(const char* msg, size_t size, uint8_t* data, size_t dataSize);

	void writeQueue();

	void readAtom(const LV2_Atom* atom);

	bool handleBuffer(uint8_t* buffer, size_t bufferSize);

protected:
	LV2_Atom_Forge* forge_;
	LV2_URID_Map* map_;

	class URIS
	{
	public:
		LV2_URID atomFloat;
		LV2_URID atomObject;
		LV2_URID atomVector;
		LV2_URID atomEventTransfer;

		LV2_URID bufsizeSequenceSize;

		LV2_URID midiEvent;
		LV2_URID timePosition;
		LV2_URID timeBarBeat;
		LV2_URID timeBPM;
		LV2_URID timeSpeed;

		LV2_URID msgOSCMsg;
		LV2_URID msgData;
		LV2_URID msgObj;
		LV2_URID msgComplete;

		LV2_URID stateKey;
		LV2_URID stateType;
	};
	URIS* uris_;

	std::queue<CommunicatorMessage*> messageQueue_;
	uint64_t msgId_;

	std::vector<std::pair<size_t, uint8_t*>> incomingQueue_;
};

}

#endif /* CR42YNTHLV2COMMUNICATOR_H */
