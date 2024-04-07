/**************************************************************************/
/*  ffmpeg_frame.h                                                        */
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

#ifndef FFMPEG_FRAME_H
#define FFMPEG_FRAME_H

#ifdef GDEXTENSION

// Headers for building as GDExtension plug-in.
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/weak_ref.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

#else

#include "core/object/ref_counted.h"

#endif
extern "C" {
#include "libavutil/frame.h"
}

class FFmpegFrame : public RefCounted {
public:
	typedef void (*return_frame_callback_t)(Ref<RefCounted> p_instance, Ref<FFmpegFrame> p_frame);

private:
	AVFrame *frame = nullptr;
	return_frame_callback_t return_func = nullptr;
	Ref<WeakRef> return_instance;

public:
	AVFrame *get_frame() const;
	double get_time() const;
	void do_return();
	FFmpegFrame(Ref<RefCounted> p_return_func_instance, return_frame_callback_t p_return_func);
	FFmpegFrame();
	~FFmpegFrame();
};

#endif // FFMPEG_FRAME_H
