#include "CR42YnthLV2Communicator.h"

#include <lv2/atom/atom.h>
#include <lv2/buf-size/buf-size.h>
#include <lv2/midi/midi.h>
#include <lv2/time/time.h>

#include "lv2_common.h"

#include "CommunicatorMessage.h"
#include "OSCEvent.h"

namespace cr42y
{

CR42YnthLV2Communicator::CR42YnthLV2Communicator() :
	forge_(new LV2_Atom_Forge()),
	map_(nullptr),
	uris_(nullptr),
	messageQueue_(),
	msgId_(0)
{

}

CR42YnthLV2Communicator::~CR42YnthLV2Communicator()
{
	delete uris_;
}

void CR42YnthLV2Communicator::initURIDMap(LV2_URID_Map* map)
{
	map_ = map;

	uris_ = new URIS();

	uris_->atomFloat = map->map(map->handle, LV2_ATOM__Float);
	uris_->atomObject = map->map(map->handle, LV2_ATOM__Object);
	uris_->atomVector = map->map(map->handle, LV2_ATOM__Vector);
	uris_->atomEventTransfer = map->map(map->handle, LV2_ATOM__eventTransfer);

	uris_->bufsizeSequenceSize = map->map(map->handle,
										  LV2_BUF_SIZE__sequenceSize);

	uris_->midiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);

	uris_->timePosition = map->map(map->handle, LV2_TIME__Position);
	uris_->timeBarBeat = map->map(map->handle, LV2_TIME__barBeat);
	uris_->timeBPM = map->map(map->handle, LV2_TIME__beatsPerMinute);
	uris_->timeSpeed = map->map(map->handle, LV2_TIME__speed);

	uris_->msgOSCMsg = map->map(map->handle, CR42Ynth__OSCMSG);
	uris_->msgData = map->map(map->handle, CR42Ynth__MSGDATA);
	uris_->msgObj = map->map(map->handle, CR42Ynth__MSGOBJ);
	uris_->msgComplete = map->map(map->handle, CR42Ynth__MSGCOMPLETE);

	uris_->stateKey = map->map(map->handle, CR42Ynth__STATEKEY);
	uris_->stateType = map->map(map->handle, CR42Ynth__STATETYPE);

	lv2_atom_forge_init(forge_, map_);
}

void CR42YnthLV2Communicator::prepareAtomWrite()
{
}

void CR42YnthLV2Communicator::finishAtomWrite(LV2_Atom* atom)
{
}

size_t CR42YnthLV2Communicator::writeMsgAtom(uint8_t* data, size_t size)
{
	size_t space = forge_->size - forge_->offset;
	
	size_t minimum = sizeof(LV2_Atom_Event) - sizeof(LV2_Atom) + lv2_atom_pad_size(
						 sizeof(LV2_Atom_Object) + sizeof(LV2_Atom_Property_Body)
						 - sizeof(LV2_Atom) + sizeof(LV2_Atom_Vector));
	
	if (space < minimum)
	{
		return 0;
	}
	
	prepareAtomWrite();

	LV2_Atom_Forge_Frame frame;

	LV2_Atom_Forge_Ref atomRef = lv2_atom_forge_object(forge_, &frame, 0, uris_->msgObj);

	lv2_atom_forge_key(forge_, uris_->msgData);

	space -= sizeof(LV2_Atom_Vector);

	space = lv2_atom_pad_size(space) - 8;

	size_t dataToWrite = std::min<size_t>(size, space);

	lv2_atom_forge_vector(forge_, dataToWrite, 0, 1, data);

	lv2_atom_forge_pop(forge_, &frame);

	finishAtomWrite(lv2_atom_forge_deref(forge_, atomRef));
	
	return dataToWrite;
}

CommunicatorMessage* CR42YnthLV2Communicator::createCommunicatorMessage(
	const char* msg, size_t size, uint8_t* data, size_t dataSize)
{
	return new CommunicatorMessage(msg, size, data, dataSize, msgId_++);
}

void CR42YnthLV2Communicator::queueMsg(const char* msg, size_t size,
									   uint8_t* data, size_t dataSize)
{
	messageQueue_.push(
		createCommunicatorMessage(msg, size, data, dataSize));
}

bool CR42YnthLV2Communicator::writeSingleMessage(CommunicatorMessage* msg)
{
	uint8_t* data = nullptr;
	
	uint32_t id = msg->id();
		
	size_t lenght = msg->read(&data);

	size_t bufferSize = sizeof(id) + lenght;
	uint8_t buffer[bufferSize];

	memcpy(buffer, &id, sizeof(id));
	memcpy(buffer + sizeof(id), data, lenght);

	size_t written = writeMsgAtom(buffer, bufferSize);

	msg->offset(written - sizeof(id));
	
	return written < bufferSize;
}

void CR42YnthLV2Communicator::writeQueue()
{
	bool done = messageQueue_.size() == 0;

	while (!done)
	{
		CommunicatorMessage* msg = messageQueue_.front();

		
		if (writeSingleMessage(msg))
		{
			done = true;
		}
		else
		{
			delete msg;
			messageQueue_.pop();
		}

		if (messageQueue_.size() == 0)
		{
			done = true;
		}
	}
}

void CR42YnthLV2Communicator::readAtom(const LV2_Atom* atom)
{
	//std::cout << "READING\n";
	if (!lv2_atom_forge_is_object_type(forge_, atom->type))
	{
		//std::cout << "fail\n";
		return;
	}

	const LV2_Atom_Object* obj = (const LV2_Atom_Object*) atom;

	LV2_Atom* dataAtom = nullptr;

	lv2_atom_object_get(obj, uris_->msgData, &dataAtom, 0);

	uint8_t* data = (uint8_t*) dataAtom + sizeof(LV2_Atom_Vector);
	size_t dataSize = dataAtom->size;

	uint32_t id = *((uint32_t*) data);
	
	//std::cout << "Handling " << id << "\n";

	for (int i = 0; i < incomingQueue_.size(); i++)
	{
		uint32_t queueID = *((uint32_t*) incomingQueue_[i].second);

		if (queueID == id)
		{
			//std::cout << "Found " << id << ", checking...\n";
			uint8_t* newBuffer = new uint8_t[incomingQueue_[i].first + dataSize
											 - sizeof(uint32_t)];

			memcpy(newBuffer, incomingQueue_[i].second,
				   incomingQueue_[i].first);
			memcpy(newBuffer + incomingQueue_[i].first, data + sizeof(uint32_t),
				   dataSize - sizeof(uint32_t));

			delete incomingQueue_[i].second;
			incomingQueue_[i].first += dataSize - sizeof(uint32_t);
			incomingQueue_[i].second = newBuffer;

			if (handleBuffer(newBuffer, incomingQueue_[i].first))
			{
				//std::cout << "success, delete\n";
				delete[] newBuffer;
				incomingQueue_.erase(incomingQueue_.begin() + i);
			}
			return;
		}
	}
	//std::cout << "Nothing found, checking...\n";
	if (!handleBuffer(data + sizeof(uint32_t), dataSize - sizeof(uint32_t)))
	{
		//std::cout << "Failed, adding...\n";
		uint8_t* buffer = new uint8_t[dataSize];
		memcpy(buffer, data, dataSize);
		incomingQueue_.push_back(std::pair<size_t, uint8_t*>(dataSize, buffer));
	}
}

bool CR42YnthLV2Communicator::handleBuffer(uint8_t* buffer, size_t bufferSize)
{
	if (bufferSize < sizeof(size_t))
	{
		//std::cout << "too small\n";
		return false;
	}

	size_t msgSize = *((size_t*)(buffer));

	if (bufferSize < sizeof(size_t) * 2 + msgSize)
	{
		//std::cout << "not done, expecting " << (sizeof(size_t) * 2 + msgSize - bufferSize) << " more bytes\n";
		//std::cout << "size " << bufferSize << "\n";
		//std::cout << "msg says " << msgSize << "\n";
		return false;
	}

	size_t dataSize = *((size_t*)(buffer + sizeof(size_t) + msgSize));

	if (bufferSize
		
		
			>= sizeof(size_t) * 2 + msgSize + dataSize)
	{
		uint8_t* data = nullptr;
		if (dataSize > 0)
		{
			data = buffer + sizeof(size_t) * 2 + msgSize;
		}
		OSCEvent event((const char*)buffer + sizeof(size_t), msgSize,
					   data,
					   dataSize);

		//std::cout << "SUCCESS\n";
		handleOSCEvent(&event);
	}

	return true;
}

}
