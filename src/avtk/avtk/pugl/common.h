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
 @addtogroup pugl
 @{
 */

#ifndef PUGL_COMMON_H_INCLUDED
#define PUGL_COMMON_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

/**
 A Pugl view.
 */
typedef struct PuglViewImpl PuglView;

/**
 A native window handle.

 On X11, this is a Window.
 On OSX, this is an NSView*.
 On Windows, this is a HWND.
 */
typedef intptr_t PuglNativeWindow;

/**
 Handle for opaque user data.
 */
typedef void* PuglHandle;

/**
 Return status code.
 */
typedef enum
{
	PUGL_SUCCESS = 0
} PuglStatus;

/**
 Drawing context type.
 */
typedef enum
{
	PUGL_GL, PUGL_CAIRO
} PuglContextType;

/**
 Convenience symbols for ASCII control characters.
 */
typedef enum
{
	PUGL_CHAR_BACKSPACE = 0x08, PUGL_CHAR_ESCAPE = 0x1B, PUGL_CHAR_DELETE = 0x7F
} PuglChar;

/**
 Keyboard modifier flags.
 */
typedef enum
{
	PUGL_MOD_SHIFT = 1, /**< Shift key */
	PUGL_MOD_CTRL = 1 << 1, /**< Control key */
	PUGL_MOD_ALT = 1 << 2, /**< Alt/Option key */
	PUGL_MOD_SUPER = 1 << 3 /**< Mod4/Command/Windows key */
} PuglMod;

/**
 Special (non-Unicode) keyboard keys.
 */
typedef enum
{
	PUGL_KEY_F1 = 1,
	PUGL_KEY_F2,
	PUGL_KEY_F3,
	PUGL_KEY_F4,
	PUGL_KEY_F5,
	PUGL_KEY_F6,
	PUGL_KEY_F7,
	PUGL_KEY_F8,
	PUGL_KEY_F9,
	PUGL_KEY_F10,
	PUGL_KEY_F11,
	PUGL_KEY_F12,
	PUGL_KEY_LEFT,
	PUGL_KEY_UP,
	PUGL_KEY_RIGHT,
	PUGL_KEY_DOWN,
	PUGL_KEY_PAGE_UP,
	PUGL_KEY_PAGE_DOWN,
	PUGL_KEY_HOME,
	PUGL_KEY_END,
	PUGL_KEY_INSERT,
	PUGL_KEY_SHIFT,
	PUGL_KEY_CTRL,
	PUGL_KEY_ALT,
	PUGL_KEY_SUPER
} PuglKey;

/**
 @}
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PUGL_COMMON_H_INCLUDED */
