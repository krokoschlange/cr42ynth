#ifndef CR42Y_LFOEDITDATA_H
#define CR42Y_LFOEDITDATA_H

#include <vector>

namespace cr42y {

class LFOEditData
{
public:
	LFOEditData();

	LFOEditData(const LFOEditData& other);

	virtual ~LFOEditData();

	LFOEditData& operator=(const LFOEditData& other);
	
	size_t getData(uint8_t** buffer);
	
	void getSamples(std::vector<float> samples);
	
	void setSamplerate(float samplerate);
	void setBPM(float bpm);
	void setTimeLength(float length);
	void setBeatsLength(float length);
	
	void setSustain(uint32_t sustain_);

private:
	std::vector<float> vertexPositions_;
	std::vector<float> vertexValues_;
	std::vector<float> handleValues_;
	
	float samplerate_;
	float timelength_;
	float bpm_;
	float beatslength_;
	
	//for envelope
	uint32_t sustain_;
};

}

#endif // CR42Y_LFOEDITDATA_H
