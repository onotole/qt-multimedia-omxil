#include "../player/player.h"
#include <cassert>

#ifdef OMX_RPI
#include <bcm_host.h>
#endif

int main(int argc, char *argv[]) {
    assert(argc >= 2);

#ifdef OMX_RPI
    bcm_host_init();
#endif

    Player player;
    player.load(argv[1]);
    player.play();

    return 0;
}
