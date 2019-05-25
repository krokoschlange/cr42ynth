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

/**
   @file pugl_internal.h Private platform-independent definitions.

   Note this file contains function definitions, so it must be compiled into
   the final binary exactly once.  Each platform specific implementation file
   including it once should achieve this.

   If you are copying the pugl code into your source tree, the following
   symbols can be defined to tweak pugl behaviour:

   PUGL_HAVE_CAIRO: Include Cairo support code.
   PUGL_HAVE_GL:    Include OpenGL support code.
*/

#include "pugl/pugl.h"
#include "pugl/event.h"

typedef struct PuglInternalsImpl PuglInternals;

struct PuglViewImpl {
	PuglHandle       handle;
	PuglEventFunc    eventFunc;
	PuglCloseFunc    closeFunc;
	PuglDisplayFunc  displayFunc;
	PuglKeyboardFunc keyboardFunc;
	PuglMotionFunc   motionFunc;
	PuglMouseFunc    mouseFunc;
	PuglReshapeFunc  reshapeFunc;
	PuglScrollFunc   scrollFunc;
	PuglSpecialFunc  specialFunc;

	PuglInternals* impl;

	PuglNativeWindow parent;
	PuglContextType  ctx_type;

	int      width;
	int      height;
	int      mods;
	bool     mouse_in_view;
	bool     ignoreKeyRepeat;
	bool     redisplay;
	bool     resizable;
	uint32_t event_timestamp_ms;
};

PuglInternals* puglInitInternals();

PuglView*
puglInit(int* pargc, char** argv)
{
	PuglView* view = (PuglView*)calloc(1, sizeof(PuglView));
	if (!view) {
		return NULL;
	}

	PuglInternals* impl = puglInitInternals();
	if (!impl) {
		return NULL;
	}

	view->impl   = impl;
	view->width  = 640;
	view->height = 480;

	return view;
}

void
puglInitWindowSize(PuglView* view, int width, int height)
{
	view->width  = width;
	view->height = height;
}

void
puglInitWindowParent(PuglView* view, PuglNativeWindow parent)
{
	view->parent = parent;
}

void
puglInitResizable(PuglView* view, bool resizable)
{
	view->resizable = resizable;
}

void
puglInitContextType(PuglView* view, PuglContextType type)
{
	view->ctx_type = type;
}

void
puglSetHandle(PuglView* view, PuglHandle handle)
{
	view->handle = handle;
}

PuglHandle
puglGetHandle(PuglView* view)
{
	return view->handle;
}

uint32_t
puglGetEventTimestamp(PuglView* view)
{
	return view->event_timestamp_ms;
}

int
puglGetModifiers(PuglView* view)
{
	return view->mods;
}

void
puglIgnoreKeyRepeat(PuglView* view, bool ignore)
{
	view->ignoreKeyRepeat = ignore;
}

void
puglSetEventFunc(PuglView* view, PuglEventFunc eventFunc)
{
	view->eventFunc = eventFunc;
}

void
puglSetCloseFunc(PuglView* view, PuglCloseFunc closeFunc)
{
	view->closeFunc = closeFunc;
}

void
puglSetDisplayFunc(PuglView* view, PuglDisplayFunc displayFunc)
{
	view->displayFunc = displayFunc;
}

void
puglSetKeyboardFunc(PuglView* view, PuglKeyboardFunc keyboardFunc)
{
	view->keyboardFunc = keyboardFunc;
}

void
puglSetMotionFunc(PuglView* view, PuglMotionFunc motionFunc)
{
	view->motionFunc = motionFunc;
}

void
puglSetMouseFunc(PuglView* view, PuglMouseFunc mouseFunc)
{
	view->mouseFunc = mouseFunc;
}

void
puglSetReshapeFunc(PuglView* view, PuglReshapeFunc reshapeFunc)
{
	view->reshapeFunc = reshapeFunc;
}

void
puglSetScrollFunc(PuglView* view, PuglScrollFunc scrollFunc)
{
	view->scrollFunc = scrollFunc;
}

void
puglSetSpecialFunc(PuglView* view, PuglSpecialFunc specialFunc)
{
	view->specialFunc = specialFunc;
}

void
puglEnterContext(PuglView* view);

void
puglLeaveContext(PuglView* view, bool flush);


const char*
puglEventGetTypeString( PuglEventType type )
{
	switch( type ) {
	case PUGL_BUTTON_PRESS:
		return "button_press"   ;
		break;
	case PUGL_BUTTON_RELEASE:
		return "button_release" ;
		break;
	case PUGL_CONFIGURE:
		return "configure"      ;
		break;
	case PUGL_EXPOSE:
		return "expose"         ;
		break;
	case PUGL_KEY_PRESS:
		return "key_press"      ;
		break;
	case PUGL_KEY_RELEASE:
		return "key_release"    ;
		break;
	case PUGL_ENTER_NOTIFY:
		return "enter_notify"   ;
		break;
	case PUGL_LEAVE_NOTIFY:
		return "leave_notify"   ;
		break;
	case PUGL_MOTION_NOTIFY:
		return "motion_notify"  ;
		break;
	case PUGL_NOTHING:
		return "nothing"        ;
		break;
	case PUGL_SCROLL:
		return "scroll"         ;
		break;
	default:
		return 0                ;
		break;
	}
	return 0;
}

static void
puglDispatchEvent(PuglView* view, const PuglEvent* event)
{
	if (view->eventFunc) {
		view->eventFunc(view, event);
	}

	switch (event->type) {
	case PUGL_CONFIGURE:
		puglEnterContext(view);
		view->width  = event->configure.width;
		view->height = event->configure.height;
		if (view->reshapeFunc) {
			view->reshapeFunc(view, view->width, view->height);
		}
		puglLeaveContext(view, false);
		break;
	case PUGL_EXPOSE:
		if (event->expose.count == 0) {
			puglEnterContext(view);
			if (view->displayFunc) {
				view->displayFunc(view);
			}
			view->redisplay = false;
			puglLeaveContext(view, true);
		}
		break;
	case PUGL_MOTION_NOTIFY:
		view->event_timestamp_ms = event->motion.time;
		view->mods               = event->motion.state;
		if (view->motionFunc) {
			view->motionFunc(view, event->motion.x, event->motion.y);
		}
		break;
	case PUGL_SCROLL:
		if (view->scrollFunc) {
			view->scrollFunc(view,
			                 event->scroll.x, event->scroll.y,
			                 event->scroll.dx, event->scroll.dy);
		}
		break;
	case PUGL_BUTTON_PRESS:
	case PUGL_BUTTON_RELEASE:
		view->event_timestamp_ms = event->button.time;
		view->mods               = event->button.state;
		if (view->mouseFunc) {
			view->mouseFunc(view,
			                event->button.button,
			                event->type == PUGL_BUTTON_PRESS,
			                event->button.x,
			                event->button.y);
		}
		break;
	case PUGL_KEY_PRESS:
	case PUGL_KEY_RELEASE:
		view->event_timestamp_ms = event->key.time;
		view->mods               = event->key.state;
		if (event->key.special && view->specialFunc) {
			view->specialFunc(view,
			                  event->type == PUGL_KEY_PRESS,
			                  event->key.special);
		} else if (event->key.character && view->keyboardFunc) {
			view->keyboardFunc(view,
			                   event->type == PUGL_KEY_PRESS,
			                   event->key.character);
		}
		break;
	default:
		break;
	}
}
