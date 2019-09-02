/*
 * Copyright(c) 2016, OpenAV
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior
 *       written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL OPENAV BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OPENAV_AVTK_DIALOG_HXX
#define OPENAV_AVTK_DIALOG_HXX

#include "group.hxx"

namespace Avtk
{

class Button;

class Dialog : public Group
{
public:
	Dialog( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
	virtual ~Dialog() {}

	enum BUTTONS {
		OK = 0,
		OK_CANCEL,
	};

	/// Optionally pass in X,Y co-ords of the mouse, and the OK/YES button will
	/// be positioned under the mouse cursor.
	void run( const char* header, const char* content, BUTTONS b = OK, int x = -1, int y = -1 );

	virtual void draw( cairo_t* cr );

private:
	BUTTONS buttons_;
	std::string contents;

	int mx, my;

	Avtk::Button* ok;
	Avtk::Button* cancel;

	// internally highjack the OK / Cancel buttons events
	virtual void valueCB( Avtk::Widget* widget);
};

};

#endif // OPENAV_AVTK_DIALOG_HXX
