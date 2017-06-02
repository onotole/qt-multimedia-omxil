#include <cassert>
#include "file_src.h"
#include "../omx/util.h"

OMX_VIDEO_CODINGTYPE av_to_omx_codec(AVCodecID id) {
    OMX_VIDEO_CODINGTYPE codec = OMX_VIDEO_CodingAutoDetect;
    // TODO: complete list
    switch (id) {
        case AV_CODEC_ID_MPEG4:
            codec = OMX_VIDEO_CodingMPEG4; break;
        case AV_CODEC_ID_H264:
            codec = OMX_VIDEO_CodingAVC; break;
        default:
            break;
    }
    return codec;
}

size_t av_to_omx_framerate(AVRational fr) {
    if (fr.den > 0 && fr.num > 0)
        return (long long) (1<<16) * fr.num / fr.den;
    return 25 * (1<<16);
}

#ifndef OMX_BUFFERFLAG_TIME_UNKNOWN
#define OMX_BUFFERFLAG_TIME_UNKNOWN 0
#endif

FileSrc::FileSrc(const std::string & file) {
    TRACE << "FileSrc ctor: " << file;
    av_register_all();
    avformat_open_input(&_context, file.c_str(), nullptr, nullptr);
    avformat_find_stream_info(_context, nullptr);
    av_dump_format(_context, 0, file.c_str(), 0);
    // TODO: select apropriate stream
    AVCodecContext * codec_context = _context->streams[0]->codec;
    _stream_config.extra_data.assign(codec_context->extradata, codec_context->extradata + codec_context->extradata_size);
    _stream_config.video_encoding = av_to_omx_codec(codec_context->codec_id);
    _stream_config.framerate = av_to_omx_framerate(codec_context->framerate);
    _stream_config.width = codec_context->width;
    _stream_config.height = codec_context->height;
}

bool FileSrc::read(omx::Buffer & buffer) {
    // do we have to read new packet ?
    if (_current_frame.size == 0) {
        while (true) {
            if (!av_read_frame(_context, &_current_frame) && _current_frame.stream_index == 0) break;
            _current_frame.reset();
        }
    }
    uint32_t todo = std::min((uint32_t)buffer.header()->nAllocLen, _current_frame.size - _current_frame.read_pos);
    memcpy((char*)buffer.data(), _current_frame.data + _current_frame.read_pos, todo);
    buffer.header()->nFilledLen = todo;
    if (!_startime_set){
        buffer.header()->nFlags |= OMX_BUFFERFLAG_STARTTIME;
    }
    buffer.header()->nFlags |= OMX_BUFFERFLAG_TIME_UNKNOWN;
    _current_frame.read_pos += todo;
    if (_current_frame.done()) {
        _current_frame.reset();
        buffer.header()->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
    }
    return true;
}

FileSrc::~FileSrc() {
   if (_context)
       avformat_close_input(&_context);
}
