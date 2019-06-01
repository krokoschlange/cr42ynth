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

#ifndef PUGL_EVENT_H_INCLUDED
#define PUGL_EVENT_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#else
#    include <stdbool.h>
#endif

#include "pugl/common.h"

/**
 The type of a PuglEvent.
 */
typedef enum
{
	PUGL_BUTTON_PRESS,
	PUGL_BUTTON_RELEASE,
	PUGL_CONFIGURE,
	PUGL_EXPOSE,
	PUGL_KEY_PRESS,
	PUGL_KEY_RELEASE,
	PUGL_ENTER_NOTIFY,
	PUGL_LEAVE_NOTIFY,
	PUGL_MOTION_NOTIFY,
	PUGL_NOTHING,
	PUGL_SCROLL
} PuglEventType;

/**
 Reason for a PuglEventCrossing.
 */
typedef enum
{
	PUGL_CROSSING_NORMAL, /**< Crossing due to pointer motion. */
	PUGL_CROSSING_GRAB, /**< Crossing due to a grab. */
	PUGL_CROSSING_UNGRAB /**< Crossing due to a grab release. */
} PuglCrossingMode;

/**
 Common header for all event structs.
 */
typedef struct
{
	PuglEventType type; /**< Event type. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
} PuglEventAny;

/**
 Button press or release event.

 For event types PUGL_BUTTON_PRESS and PUGL_BUTTON_RELEASE.
 */
typedef struct
{
	PuglEventType type; /**< PUGL_BUTTON_PRESS or PUGL_BUTTON_RELEASE. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	uint32_t time; /**< Time in milliseconds. */
	double x; /**< View-relative X coordinate. */
	double y; /**< View-relative Y coordinate. */
	double x_root; /**< Root-relative X coordinate. */
	double y_root; /**< Root-relative Y coordinate. */
	unsigned state; /**< Bitwise OR of PuglMod flags. */
	unsigned button; /**< 1-relative button number. */
} PuglEventButton;

/**
 Configure event for when window size or position has changed.
 */
typedef struct
{
	PuglEventType type; /**< PUGL_CONFIGURE. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	double x; /**< New parent-relative X coordinate. */
	double y; /**< New parent-relative Y coordinate. */
	double width; /**< New width. */
	double height; /**< New height. */
} PuglEventConfigure;

/**
 Expose event for when a region must be redrawn.
 */
typedef struct
{
	PuglEventType type; /**< PUGL_EXPOSE. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	double x; /**< View-relative X coordinate. */
	double y; /**< View-relative Y coordinate. */
	double width; /**< Width of exposed region. */
	double height; /**< Height of exposed region. */
	int count; /**< Number of expose events to follow. */
} PuglEventExpose;

/**
 Key press/release event.

 Keys that correspond to a Unicode character are expressed as a character
 code.  For other keys, `character` will be 0 and `special` indicates the key
 pressed.
 */
typedef struct
{
	PuglEventType type; /**< PUGL_KEY_PRESS or PUGL_KEY_RELEASE. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	uint32_t time; /**< Time in milliseconds. */
	double x; /**< View-relative X coordinate. */
	double y; /**< View-relative Y coordinate. */
	double x_root; /**< Root-relative X coordinate. */
	double y_root; /**< Root-relative Y coordinate. */
	unsigned state; /**< Bitwise OR of PuglMod flags. */
	uint32_t character; /**< Unicode character code, or 0. */
	PuglKey special; /**< Special key, if character is 0. */
} PuglEventKey;

/**
 Pointer crossing event (enter and leave).
 */
typedef struct
{
	PuglEventType type; /**< PUGL_ENTER_NOTIFY or PUGL_LEAVE_NOTIFY. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	uint32_t time; /**< Time in milliseconds. */
	double x; /**< View-relative X coordinate. */
	double y; /**< View-relative Y coordinate. */
	double x_root; /**< Root-relative X coordinate. */
	double y_root; /**< Root-relative Y coordinate. */
	unsigned state; /**< Bitwise OR of PuglMod flags. */
	PuglCrossingMode mode; /**< Reason for crossing. */
} PuglEventCrossing;

/**
 Pointer motion event.
 */
typedef struct
{
	PuglEventType type; /**< PUGL_MOTION_NOTIFY. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	uint32_t time; /**< Time in milliseconds. */
	double x; /**< View-relative X coordinate. */
	double y; /**< View-relative Y coordinate. */
	double x_root; /**< Root-relative X coordinate. */
	double y_root; /**< Root-relative Y coordinate. */
	unsigned state; /**< Bitwise OR of PuglMod flags. */
	bool is_hint; /**< True iff this event is a motion hint. */
	bool focus; /**< True iff this is the focused window. */
} PuglEventMotion;

/**
 Scroll event.

 The scroll distance is expressed in "lines", an arbitrary unit that
 corresponds to a single tick of a detented mouse wheel.  For example, `dy` =
 1.0 scrolls 1 line up.  Some systems and devices support finer resolution
 and/or higher values for fast scrolls, so programs should handle any value
 gracefully.
 */
typedef struct
{
	PuglEventType type; /**< PUGL_SCROLL. */
	PuglView* view; /**< View that received this event. */
	bool send_event; /**< True iff event was sent explicitly. */
	uint32_t time; /**< Time in milliseconds. */
	double x; /**< View-relative X coordinate. */
	double y; /**< View-relative Y coordinate. */
	double x_root; /**< Root-relative X coordinate. */
	double y_root; /**< Root-relative Y coordinate. */
	unsigned state; /**< Bitwise OR of PuglMod flags. */
	double dx; /**< Scroll X distance in lines. */
	double dy; /**< Scroll Y distance in lines. */
} PuglEventScroll;

/**
 Interface event.

 This is a union of all event structs.  The `type` must be checked to
 determine which fields are safe to access.  A pointer to PuglEvent can
 either be cast to the appropriate type, or the union members used.
 */
typedef union
{
	PuglEventType type; /**< Event type. */
	PuglEventAny any; /**< Valid for all event types. */
	PuglEventButton button; /**< PUGL_BUTTON_PRESS, PUGL_BUTTON_RELEASE. */
	PuglEventConfigure configure; /**< PUGL_CONFIGURE. */
	PuglEventCrossing crossing; /**< PUGL_ENTER_NOTIFY, PUGL_LEAVE_NOTIFY. */
	PuglEventExpose expose; /**< PUGL_EXPOSE. */
	PuglEventKey key; /**< PUGL_KEY_PRESS, PUGL_KEY_RELEASE. */
	PuglEventMotion motion; /**< PUGL_MOTION_NOTIFY. */
	PuglEventScroll scroll; /**< PUGL_SCROLL. */
} PuglEvent;

/**
 @}
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PUGL_EVENT_H_INCLUDED */
