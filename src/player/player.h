#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <memory>
#include "sink/sink.h"
#include "src/src.h"

class Player {
public:
    Player();

    void load(const std::string & uri);
    void play();
    void pause();

private:

    std::unique_ptr<Source> _src;
    std::unique_ptr<ESSink> _sink;
    void * _buffer;
};

#endif // __PLAYER_H__
