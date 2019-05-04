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
   @file pugl.h API for Pugl, a minimal portable API for OpenGL.
*/

#ifndef PUGL_H_INCLUDED
#define PUGL_H_INCLUDED

#include <stdint.h>

#include "pugl/common.h"
#include "pugl/event.h"

#ifdef PUGL_SHARED
#    ifdef _WIN32
#        define PUGL_LIB_IMPORT __declspec(dllimport)
#        define PUGL_LIB_EXPORT __declspec(dllexport)
#    else
#        define PUGL_LIB_IMPORT __attribute__((visibility("default")))
#        define PUGL_LIB_EXPORT __attribute__((visibility("default")))
#    endif
#    ifdef PUGL_INTERNAL
#        define PUGL_API PUGL_LIB_EXPORT
#    else
#        define PUGL_API PUGL_LIB_IMPORT
#    endif
#else
#    define PUGL_API
#endif

#ifdef __cplusplus
extern "C" {
#else
#    include <stdbool.h>
#endif

/**
   @defgroup pugl Pugl
   A minimal portable API for OpenGL.
   @{
*/

/**
   A function called when an event occurs.
*/
typedef void (*PuglEventFunc)(PuglView* view, const PuglEvent* event);

/**
   A function called when the window is closed.
*/
typedef void (*PuglCloseFunc)(PuglView* view);

/**
   A function called to draw the view contents with OpenGL.
*/
typedef void (*PuglDisplayFunc)(PuglView* view);

/**
   A function called when a key is pressed or released.
   @param view The view the event occured in.
   @param press True if the key was pressed, false if released.
   @param key Unicode point of the key pressed.
*/
typedef void (*PuglKeyboardFunc)(PuglView* view, bool press, uint32_t key);

/**
   A function called when the pointer moves.
   @param view The view the event occured in.
   @param x The window-relative x coordinate of the pointer.
   @param y The window-relative y coordinate of the pointer.
*/
typedef void (*PuglMotionFunc)(PuglView* view, int x, int y);

/**
   A function called when a mouse button is pressed or released.
   @param view The view the event occured in.
   @param button The button number (1 = left, 2 = middle, 3 = right).
   @param press True if the key was pressed, false if released.
   @param x The window-relative x coordinate of the pointer.
   @param y The window-relative y coordinate of the pointer.
*/
typedef void (*PuglMouseFunc)(
        PuglView* view, int button, bool press, int x, int y);

/**
   A function called when the view is resized.
   @param view The view being resized.
   @param width The new view width.
   @param height The new view height.
*/
typedef void (*PuglReshapeFunc)(PuglView* view, int width, int height);

/**
   A function called on scrolling (e.g. mouse wheel or track pad).

   The distances used here are in "lines", a single tick of a clicking mouse
   wheel.  For example, @p dy = 1.0 scrolls 1 line up.  Some systems and
   devices support finer resolution and/or higher values for fast scrolls,
   so programs should handle any value gracefully.

   @param view The view being scrolled.
   @param dx The scroll x distance.
   @param dx The scroll y distance.
*/
typedef void (*PuglScrollFunc)(PuglView* view,
                               int       x,
                               int       y,
                               float     dx,
                               float     dy);

/**
   A function called when a special key is pressed or released.

   This callback allows the use of keys that do not have unicode points.

   @param view The view the event occured in.
   @param press True if the key was pressed, false if released.
   @param key The key pressed.
*/
typedef void (*PuglSpecialFunc)(PuglView* view, bool press, PuglKey key);

/**
   @name Initialization
   Configuration functions which must be called before creating a window.
   @{
*/

/**
   Create a Pugl context.

   To create a window, call the various puglInit* functions as necessary, then
   call puglCreateWindow().

   @param pargc Pointer to argument count (unused, for GLUT compatibility).
   @param argv  Arguments (unused, for GLUT compatibility).
*/
PUGL_API PuglView*
puglInit(int* pargc, char** argv);

/**
   Set the parent window before creating a window (for embedding).
*/
PUGL_API void
puglInitWindowParent(PuglView* view, PuglNativeWindow parent);

/**
   Set the window size before creating a window.
*/
PUGL_API void
puglInitWindowSize(PuglView* view, int width, int height);

/**
   Enable or disable resizing before creating a window.
*/
PUGL_API void
puglInitResizable(PuglView* view, bool resizable);

/**
   Set the context type before creating a window.
*/
PUGL_API void
puglInitContextType(PuglView* view, PuglContextType type);

/**
   @}
*/

/**
   @name Windows
   Window management functions.
   @{
*/

/**
   Create a window with the settings given by the various puglInit functions.

   @return 1 (pugl does not currently support multiple windows).
*/
PUGL_API int
puglCreateWindow(PuglView* view, const char* title);

/**
   Show the current window.
*/
PUGL_API void
puglShowWindow(PuglView* view);

/**
   Hide the current window.
*/
PUGL_API void
puglHideWindow(PuglView* view);

/**
   Return the native window handle.
*/
PUGL_API PuglNativeWindow
puglGetNativeWindow(PuglView* view);

/**
   @}
*/

/**
   Set the handle to be passed to all callbacks.

   This is generally a pointer to a struct which contains all necessary state.
   Everything needed in callbacks should be here, not in static variables.

   Note the lack of this facility makes GLUT unsuitable for plugins or
   non-trivial programs; this mistake is largely why Pugl exists.
*/
PUGL_API void
puglSetHandle(PuglView* view, PuglHandle handle);

/**
   Get the handle to be passed to all callbacks.
*/
PUGL_API PuglHandle
puglGetHandle(PuglView* view);

/**
   Get the drawing context.

   For PUGL_GL contexts, this is unused and returns NULL.
   For PUGL_CAIRO contexts, this returns a pointer to a cairo_t.
*/
PUGL_API void*
puglGetContext(PuglView* view);

/**
   Return the timestamp (if any) of the currently-processing event.
*/
PUGL_API uint32_t
puglGetEventTimestamp(PuglView* view);

/**
   Get the currently active modifiers (PuglMod flags).

   This should only be called from an event handler.
*/
PUGL_API int
puglGetModifiers(PuglView* view);

/**
   Get a string of the event type
*/
PUGL_API const char*
puglEventGetTypeString( PuglEventType type );


/**
   Ignore synthetic repeated key events.
*/
PUGL_API void
puglIgnoreKeyRepeat(PuglView* view, bool ignore);

/**
   @name Event Callbacks
   Functions to set event callbacks for handling user input.
   @{
*/

/**
   Set the function to call when an event occurs.
*/
PUGL_API void
puglSetEventFunc(PuglView* view, PuglEventFunc eventFunc);

/**
   Set the function to call when the window is closed.
*/
PUGL_API void
puglSetCloseFunc(PuglView* view, PuglCloseFunc closeFunc);

/**
   Set the display function which should draw the UI using GL.
*/
PUGL_API void
puglSetDisplayFunc(PuglView* view, PuglDisplayFunc displayFunc);

/**
   Set the function to call on keyboard events.
*/
PUGL_API void
puglSetKeyboardFunc(PuglView* view, PuglKeyboardFunc keyboardFunc);

/**
   Set the function to call on mouse motion.
*/
PUGL_API void
puglSetMotionFunc(PuglView* view, PuglMotionFunc motionFunc);

/**
   Set the function to call on mouse button events.
*/
PUGL_API void
puglSetMouseFunc(PuglView* view, PuglMouseFunc mouseFunc);

/**
   Set the function to call on scroll events.
*/
PUGL_API void
puglSetScrollFunc(PuglView* view, PuglScrollFunc scrollFunc);

/**
   Set the function to call on special events.
*/
PUGL_API void
puglSetSpecialFunc(PuglView* view, PuglSpecialFunc specialFunc);

/**
   Set the function to call when the window size changes.
*/
PUGL_API void
puglSetReshapeFunc(PuglView* view, PuglReshapeFunc reshapeFunc);

/**
   @}
*/

/**
   Grab the input focus.
*/
PUGL_API void
puglGrabFocus(PuglView* view);

/**
   Process all pending window events.

   This handles input events as well as rendering, so it should be called
   regularly and rapidly enough to keep the UI responsive.
*/
PUGL_API PuglStatus
puglProcessEvents(PuglView* view);

/**
   Request a redisplay on the next call to puglProcessEvents().
*/
PUGL_API void
puglPostRedisplay(PuglView* view);

/**
   Destroy a GL window.
*/
PUGL_API void
puglDestroy(PuglView* view);

/**
   @}
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PUGL_H_INCLUDED */
