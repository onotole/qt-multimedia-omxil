#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <memory>
#include <thread>
#include "sink/sink.h"
#include "src/src.h"

class Player {
public:
    Player();
    ~Player();

    void load(const std::string & uri);
    void unload();
    void play();
    void pause();

private:
    std::thread _data_thread;
    std::mutex _lock;
    std::unique_ptr<Source> _src;
    std::unique_ptr<ESSink> _sink;
};

#endif // __PLAYER_H__
