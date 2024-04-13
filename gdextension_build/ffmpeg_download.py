import os
import pathlib
import shutil
import urllib.request
import tarfile
import SCons

ffmpeg_versions = {
    "avcodec": "60",
    "avfilter": "9",
    "avformat": "60",
    "avutil": "58",
    "swresample": "4",
    "swscale": "7",
}


def get_ffmpeg_install_targets(env, target_dir):
    if env["platform"] == "linuxbsd" or env["platform"] == "linux":
        return [os.path.join(target_dir, f"lib{lib}.so.{version}") for lib, version in ffmpeg_versions.items()]
    elif env["platform"] == "macos":
        return [os.path.join(target_dir, f"lib{lib}.{version}.dylib") for lib, version in ffmpeg_versions.items()]
    else:
        return [os.path.join(target_dir, f"{lib}-{version}.dll") for lib, version in ffmpeg_versions.items()]


def get_ffmpeg_install_sources(env, source_dir):
    if env["platform"] == "linuxbsd" or env["platform"] == "linux":
        return [os.path.join(source_dir, f"lib/lib{lib}.so") for lib in ffmpeg_versions]
    elif env["platform"] == "macos":
        return [os.path.join(source_dir, f"lib/lib{lib}.dylib") for lib in ffmpeg_versions]
    else:
        return [os.path.join(source_dir, f"bin/{lib}-{version}.dll") for lib, version in ffmpeg_versions.items()]

def _ffmpeg_emitter(target, source, env):
    target += get_ffmpeg_install_sources(env, os.path.dirname(target[0].get_path()))
    if env["platform"] == "windows":
        target += [
            os.path.join(os.path.dirname(target[0].get_path()), f"lib/{lib}.lib")
            for lib, version in ffmpeg_versions.items()
        ]
    else:
        target += [
            os.path.join(os.path.dirname(target[0].get_path()), f"lib/lib{lib}.so")
            for lib, version in ffmpeg_versions.items()
        ]

    emitter_headers = [
        "libavcodec/codec.h",
        "libavcodec/avcodec.h",
        "libavutil/frame.h",
        "libavformat/avformat.h",
        "libavformat/avio.h",
        "libswresample/swresample.h",
        "libswscale/swscale.h",
    ]

    target += [os.path.join(os.path.dirname(target[0].get_path()), "include/" + x) for x in emitter_headers]

    return target, source

def ffmpeg_install(env, target, source):
    return env.InstallAs(get_ffmpeg_install_targets(env, target), get_ffmpeg_install_sources(env, source))
