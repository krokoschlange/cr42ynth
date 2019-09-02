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

#include <string>
#include <iostream>


#include "WavetableEditData.h"
#include "TestUI.h"
#include "WPFunction.h"
#include "WPHarmonics.h"

int main()
{
	cr42y::TestUI* ui = new cr42y::TestUI(0);

	ui->run();
	delete ui;
	
	/*cr42y::WavetableEditData eD(32);
	eD.removePart(0, 0);
	cr42y::harmonicTable_t ht;
	ht.push_back(std::pair<float, float>(0, 0));
	ht.push_back(std::pair<float, float>(1, 0));
	ht.push_back(std::pair<float, float>(1, 0.5));
	ht.push_back(std::pair<float, float>(0.75, 0.2));
	eD.addPart(0, new cr42y::WPHarmonics(0, 1, ht, cr42y::WPHarmonics::SQR));
	eD.addPart(0, new cr42y::WPFunction(0.1, 0.5, "x"));
	
	//std::cout << eD.to_string();
	
	void* d = nullptr;
	int s = eD.getData(&d);
	
	for (int i = 0; i < s; i++)
	{
	//	std::cout << (int) ((char*) d)[i] << "|";
	}
	
	cr42y::WavetableEditData* eD2 = new cr42y::WavetableEditData((char*) d);
	std::cout << eD2->to_string();
	
	/*cr42y::harmonicTable_t* ht2 = ((cr42y::WPHarmonics*) eD2->getPartByIndex(0, 0))->getHarmonicTable();
	for (int i = 0; i < 5; i++)
	{
		std::cout << "a: " << (*ht2)[i].first << ", p: " << (*ht2)[i].second << "\n";
	}
	*/



	return 0;
}

