#include <map>
#include <algorithm>
#include "src.h"
#include "file_src.h"

enum class Proto {
    UNKNOWN = 0,
    FILE,
    HTTP,
    RTP,
    RTSP
};

Proto parse_protocol(const std::string & uri) {
    auto pos = uri.find("://");
    if (pos == std::string::npos)
        throw std::runtime_error("parse error");
    auto proto = uri.substr(0, pos);
    if (proto == "file") return Proto::FILE;
    if (proto == "http") return Proto::HTTP;
    if (proto == "rtp")  return Proto::RTP;
    if (proto == "rtsp") return Proto::RTSP;
    throw std::runtime_error("parse error");
}

Source * Source::create(const std::string & uri) {
    try {
        auto proto = parse_protocol(uri);
        // TODO: switch to different sources?
        switch(proto) {
            default:
                return new FileSrc(uri);
        }
    } catch (...) {
        return nullptr;
    }
}
