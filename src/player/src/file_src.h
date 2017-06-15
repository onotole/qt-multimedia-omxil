#ifndef __FILE_SRC_H__
#define __FILE_SRC_H__

#include "src.h"

extern "C" {
#include <libavformat/avformat.h>
}

struct AVPacketWrap : AVPacket {
    AVPacketWrap() {
        av_init_packet(this);
        data = nullptr;
        size = 0;
    }
    ~AVPacketWrap() {
        reset();
    }
    void reset() {
        av_free_packet(this);
        av_init_packet(this);
        read_pos = 0;
    }
    bool done() const {
        return read_pos >= (size_t)size;
    }
    uint32_t read_pos = 0;
};

class FileSrc : public PullSource {
public:
    FileSrc (const std::string & file);
    virtual ~FileSrc();

    const stream_config_t & stream_config() const override {
        return _stream_config;
    }
    bool read(omx::Buffer &) override;

private:
    AVFormatContext * _context = nullptr;
    stream_config_t _stream_config;
    AVPacketWrap _current_frame;
    size_t _video_idx = -1;
    bool _startime_set = false;
};


#endif // __FILE_SRC_H__
