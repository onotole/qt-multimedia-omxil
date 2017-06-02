#ifndef __SRC_H__
#define __SRC_H__

#include <string>
#include <functional>
#include <vector>
#include "../omx/component.h"

struct Source {
    struct stream_config_t {
        OMX_VIDEO_CODINGTYPE video_encoding;
        uint32_t framerate;
        uint32_t width;
        uint32_t height;
        std::vector<uint8_t> extra_data;
    };

    static Source * create(const std::string & uri);

    enum class SrcMode { PULL, PUSH };
    virtual SrcMode mode() const = 0;
    virtual const stream_config_t & stream_config() const = 0;
};

struct PushSource : Source {
    typedef std::function<void()> data_callback_t;
    virtual SrcMode mode() const override { return SrcMode::PUSH; }
    virtual void set_data_handler(data_callback_t &&) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};

struct PullSource : Source {
    virtual SrcMode mode() const override { return SrcMode::PULL; }
    virtual bool read(omx::Buffer & buffer) = 0;
};

#endif // __SRC_H__
