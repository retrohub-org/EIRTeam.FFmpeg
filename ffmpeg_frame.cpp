/**************************************************************************/
/*  ffmpeg_frame.cpp                                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             EIRTeam.FFmpeg                             */
/*                         https://ph.eirteam.moe                         */
/**************************************************************************/
/* Copyright (c) 2023-present Álex Román (EIRTeam) & contributors.        */
/*                                                                        */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "ffmpeg_frame.h"

#ifdef GDEXTENSION
#include <godot_cpp/variant/utility_functions.hpp>
#endif

AVFrame *FFmpegFrame::get_frame() const {
	return frame;
}

void FFmpegFrame::do_return() {
	if (return_func && return_instance.is_valid()) {
		Ref<RefCounted> instance_ref = return_instance->get_ref();
		if (!instance_ref.is_valid()) {
			return;
		}
		return_func(instance_ref, this);
	}
}

FFmpegFrame::FFmpegFrame(Ref<RefCounted> p_return_func_instance, return_frame_callback_t p_return_func) {
	if (p_return_func && p_return_func_instance.is_valid()) {
		return_func = p_return_func;
#ifdef GDEXTENSION
		return_instance = UtilityFunctions::weakref(p_return_func_instance);
#else
		return_instance.instantiate();
		return_instance->set_ref(p_return_func_instance);
		return_instance = p_return_func_instance;
#endif
	}
	frame = av_frame_alloc();
}

FFmpegFrame::FFmpegFrame() {
	frame = av_frame_alloc();
}

FFmpegFrame::~FFmpegFrame() {
	av_frame_free(&frame);
}
