/*******************************************************************************
 * Copyright (c) 2020 krokoschlange and contributors.
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

#ifndef LV2_COMMON_H
#define LV2_COMMON_H

#define CR42Ynth__URI "https://github.com/krokoschlange/cr42ynth"
#define CR42Ynth__DSP CR42Ynth__URI "#dsp"
#define CR42Ynth__UI CR42Ynth__URI "#ui"

#define CR42Ynth__OSCMSG CR42Ynth__URI "#msg_oscmsg"
#define CR42Ynth__MSGDATA CR42Ynth__URI "#msg_data"
#define CR42Ynth__MSGOBJ CR42Ynth__URI "#msg_obj"
#define CR42Ynth__MSGCOMPLETE CR42Ynth__URI "#msg_complete"

#define CR42Ynth__STATEKEY CR42Ynth__URI "#state_key"
#define CR42Ynth__STATETYPE CR42Ynth__URI "#state_type"

namespace cr42y
{
enum MsgAtomCompletionStage
{
	HAS_DATA_START,
	HAS_DATA_END
};
}


#endif /* LV2_COMMON_H */
