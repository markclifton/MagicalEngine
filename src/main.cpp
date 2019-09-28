#ifdef _WIN32
#include <Windows.h>
#endif

#include "game/instance.h"

int main(int argc, char* argv[]) {
#ifdef _WIN32
    FreeConsole();
#endif
    ME::Game::Instance().run();
    return 0;
}