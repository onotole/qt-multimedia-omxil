#include <map>
#include <algorithm>
#include "src.h"
#include "file_src.h"

struct Uri {
    std::string protocol;
    std::string path;
    std::string host;
    std::string port;
    std::map<std::string, std::string> params;
};

Uri parse_uri(const std::string & uri) {
    Uri parsed;

    auto pos = uri.find("://");
    if (pos == std::string::npos)
        throw std::runtime_error("parse error");

    parsed.protocol = uri.substr(0, pos);
    if (parsed.protocol == "file") {
        parsed.path = uri.substr(pos + 2);
        return parsed;
    }
    throw std::runtime_error("parse error");
}

Source * Source::create(const std::string & uri) {
    try {
        Uri parsed = parse_uri(uri);
        return new FileSrc(parsed.path);
    } catch (...) {
        return nullptr;
    }
}
