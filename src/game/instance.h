#pragma once

#include <memory>

#include "window.h"

namespace ME { namespace Game {
    class Instance {
    public:
        Instance();

        void run();
    protected:
    private:
        void load();
        void update();

        std::unique_ptr<Window> m_window;
    };
}}