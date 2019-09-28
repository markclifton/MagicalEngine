#include "game/instance.h"
#include "utils/log.h"

int main(int argc, char* argv[]) {
    ME::Game::Instance().run();
    return 0;
}