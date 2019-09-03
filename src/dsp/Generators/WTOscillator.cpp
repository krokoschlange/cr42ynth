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

#include <cmath>
#include <cstring>

#include "rtosc/rtosc.h"

#include "WTOscillator.h"
#include "CR42YnthDSP.h"

namespace cr42y
{

WTOscillator::WTOscillator(std::vector<Voice*>* vce, int id, float rate) :
		Generator(vce),
		number(id),
		samplerate(rate),
		wavetable(nullptr),
		active("/oscillators/" + std::to_string(id) + "/active", CR42YnthDSP::getInstance()->getCommunicator()),
		smooth("/oscillators/" + std::to_string(id) + "/smooth", CR42YnthDSP::getInstance()->getCommunicator()),
		noise("/oscillators/" + std::to_string(id) + "/noise", CR42YnthDSP::getInstance()->getCommunicator()),
		volume("/oscillators/" + std::to_string(id) + "/volume", CR42YnthDSP::getInstance()->getCommunicator(), 1),
		detune("/oscillators/" + std::to_string(id) + "/detune", CR42YnthDSP::getInstance()->getCommunicator(), 0, -96, 96),
		pan("/oscillators/" + std::to_string(id) + "/pan", CR42YnthDSP::getInstance()->getCommunicator(), 0, -1, 1),
		noteShift("/oscillators/" + std::to_string(id) + "/note_shift", CR42YnthDSP::getInstance()->getCommunicator(), 0, -24, 24),
		wtPos("/oscillators/" + std::to_string(id) + "/wt_pos", CR42YnthDSP::getInstance()->getCommunicator()),
		unisonAmount("/oscillators/" + std::to_string(id) + "/unison_amount", CR42YnthDSP::getInstance()->getCommunicator(), 1, 1, 16),
		unisonDetune("/oscillators/" + std::to_string(id) + "/unison_detune", CR42YnthDSP::getInstance()->getCommunicator(), 0, -8, 8),
		unisonSpread("/oscillators/" + std::to_string(id) + "/unison_spread", CR42YnthDSP::getInstance()->getCommunicator()),
		phaseShift("/oscillators/" + std::to_string(id) + "/phase_shift", CR42YnthDSP::getInstance()->getCommunicator(), 0, -1, 1),
		phaseRand("/oscillators/" + std::to_string(id) + "/phase_rand", CR42YnthDSP::getInstance()->getCommunicator()),
		editData(new WavetableEditData(4096))
{
	setWavetable(editData->getSamples());
}

WTOscillator::~WTOscillator()
{
	if (wavetable)
	{
		delete wavetable;
	}
}

void WTOscillator::setWavetable(std::vector<std::vector<float>>* wt)
{
	if (wavetable)
	{
		delete wavetable;
	}
	wavetable = wt;
}

void WTOscillator::setEditData(WavetableEditData* ed)
{
	if (editData)
	{
		delete editData;
	}
	editData = ed;
	CR42YnthDSP::getInstance()->getCommunicator()->log(editData->to_string().c_str());
	if (editData)
	{
		setWavetable(editData->getSamples());
		std::string str = "";
		for (int i = 0; i < (*wavetable)[0].size(); i += 400)
		{
			str += std::to_string((*wavetable)[0][i]);
		}
		CR42YnthDSP::getInstance()->getCommunicator()->log(str.c_str());
		CR42YnthDSP::getInstance()->getCommunicator()->log(std::to_string(ed->getWidth()).c_str());
	}
	else
	{
		setWavetable(nullptr);
	}
}

void WTOscillator::nextSample()
{
	values.clear();
	output.clear();

	for (int vce = 0; vce < voiceData.size(); vce++)
	{
		float left = 0;
		float right = 0;
		OscillatorVoiceData* voice = voiceData[vce];
		for (int uni = 0; uni < voice->phases.size(); uni++)
		{
			float val = 0;
			if (noise.getValue())
			{
				val = (rand() % 200) / 100. - 1;
			}
			else
			{
				if (wavetable)
				{
					float waveSample = (voice->phases[uni] + voice->phaseShift.getValue() + voice->PM);
					waveSample =
							waveSample >= 0 ?
									waveSample :
									1 + (waveSample - (int) waveSample); //normalize
					waveSample =
							waveSample <= 1 ? waveSample :
												waveSample - (int) waveSample;

					waveSample *= (*wavetable)[0].size();

					int wtSample = (int) voice->wtPos.getValue() * wavetable->size();
					if (wtSample >= wavetable->size())
					{
						wtSample = wavetable->size() - 1;
					}
					else if (wtSample < 0)
					{
						wtSample = 0;
					}
					if (smooth.getValue())
					{
						float smpl1 = (*wavetable)[wtSample][(int) waveSample];
						float waveSample2 = waveSample + 1;
						if (waveSample2 >= (*wavetable)[0].size())
						{
							waveSample2 -= (*wavetable)[0].size();
						}
						float smpl2 = (*wavetable)[wtSample][(int) waveSample2];
						val = smpl1 + (waveSample - (int) waveSample) * (smpl2 - smpl1);
					}
					else
					{
						val = (*wavetable)[wtSample][(int) waveSample];
					}
				}
			}
			if (uni == 0)
			{
				values.insert(std::pair<Voice*, float>(voice->voice, val));
			}

			float panFactor = voice->pan.getValue();
			if (voice->phases.size() > 1)
			{
				float uniSpread = voice->unisonSpread.getValue();
				panFactor += -uniSpread + uni * ((uniSpread * 2) / voice->phases.size() - 1);
			}

			float lPan = (sqrtf(2) / 2) * (cosf(panFactor * M_PI_4) + sinf(panFactor * M_PI_4));
			float rPan = (sqrtf(2) / 2) * (cosf(panFactor * M_PI_4) - sinf(panFactor * M_PI_4));

			left += val * voice->volume.getValue() * lPan * voice->AM * voice->RM * voice->voice->getVelocity();
			right += val * voice->volume.getValue() * rPan * voice->AM * voice->RM * voice->voice->getVelocity();

			float note = voice->voice->getNote() + detune.getValue() + voice->noteShift.getValue();
			if (voice->phases.size() > 1)
			{
				float uniDet = voice->unisonDetune.getValue();
				note += -uniDet + uni * ((uniDet * 2) / voice->phases.size() - 1);
			}
			float frequency = pow(2, (note - 69) / 12) * 440 * voice->FM;
			float deltaPhase = frequency / samplerate;
			voice->phases[uni] = voice->phases[uni] + deltaPhase;

			if (voice->phases[uni] >= 1)
			{
				voice->phases[uni] -= (int) voice->phases[uni];
			}
		}
		std::vector<float> out;
		out.push_back(left);
		out.push_back(right);
		output.insert(std::pair<Voice*, std::vector<float>>(voice->voice, out));
	}
}

void WTOscillator::voiceAdded(Voice* vce)
{
	voiceData.push_back(new OscillatorVoiceData(this, vce));
}

void WTOscillator::voiceRemoved(Voice* vce)
{
	for (int i = 0; i < voiceData.size(); i++)
	{
		if (voiceData[i]->voice == vce)
		{
			delete voiceData[i];
			voiceData.erase(voiceData.begin() + i);
			i--;
		}
	}
}

void WTOscillator::sendState()
{
	std::string address = "oscillators/" + std::to_string(number) + "/wavetable";

	unsigned int bufferSize = address.size() + 32;
	char buffer[bufferSize];

	int len = rtosc_message(buffer, bufferSize, address.c_str(), "s", "set");

	if (editData)
	{
		/*float wtValues[width * height];
		 for (int i = 0; i < height; i++)
		 {
		 for (int j = 0; j < width; j++)
		 {
		 wtValues[i * width + j] = (*wavetable)[i][j];
		 }
		 }
		 CR42YnthDSP::getInstance()->getCommunicator()->writeMessage(buffer, len, (void*) wtValues, width * height * sizeof(float));
		 */
		void* dataBuffer = nullptr;
		int dataSize = editData->getData(&dataBuffer);
		CR42YnthDSP::getInstance()->getCommunicator()->writeMessage(buffer, len, dataBuffer, dataSize);
	}
	else
	{
		CR42YnthDSP::getInstance()->getCommunicator()->writeMessage(buffer, len, nullptr, 0);
	}
}

bool WTOscillator::receiveOSCMessage(OSCEvent* event)
{
	const char* msg = event->getMessage();
	std::string pattern = "/oscillators/" + std::to_string(number) + "/wavetable";
	char* end = nullptr;
	rtosc_match_path(pattern.c_str(), msg, (const char**) &end);
	if (end && *end == '\0')
	{
		if (rtosc_type(msg, 0) == 's' && !strcmp(rtosc_argument(msg, 0).s, "set"))
		{
			if (event->getData())
			{
				setEditData(new WavetableEditData((char*) event->getData()));
			}
			else
			{
				setEditData(nullptr);
			}
			return true;
		}
		else if (rtosc_type(msg, 0) == 's' && !strcmp(rtosc_argument(msg, 0).s, "update"))
		{
			if (editData && event->getData())
			{
				editData->update((char*) event->getData());
				setWavetable(editData->getSamples());
			}
		}
	}
	return false;
}

void WTOscillator::midiPanic()
{
	for (int i = 0; i < voiceData.size(); i++)
	{
		delete voiceData[i];
	}
	voiceData.clear();
}

std::vector<float> WTOscillator::getOutput(Voice* vce)
{
	std::map<Voice*, std::vector<float>>::iterator it = output.find(vce);
	if (it != output.end())
	{
		return it->second;
	}
	return std::vector<float>();
}

Control* WTOscillator::getActiveCtrl()
{
	return &active;
}

Control* WTOscillator::getSmoothCtrl()
{
	return &smooth;
}

Control* WTOscillator::getVolumeCtrl()
{
	return &volume;
}

Control* WTOscillator::getPanCtrl()
{
	return &pan;
}

Control* WTOscillator::getNoteShiftCtrl()
{
	return &noteShift;
}

Control* WTOscillator::getWTPosCtrl()
{
	return &wtPos;
}

Control* WTOscillator::getUnisonAmountCtrl()
{
	return &unisonAmount;
}

Control* WTOscillator::getUnisonDetuneCtrl()
{
	return &unisonDetune;
}

Control* WTOscillator::getUnisonSpreadCtrl()
{
	return &unisonSpread;
}

Control* WTOscillator::getPhaseShiftCtrl()
{
	return &phaseShift;
}

Control* WTOscillator::getPhaseRandCtrl()
{
	return &phaseRand;
}

} /* namespace cr42y */
