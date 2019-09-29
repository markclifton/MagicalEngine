#pragma once

#include <memory>

#include "window.h"
#include "utils/non_copyable.h"

namespace ME { namespace Game {
    class Instance : NonCopyable {
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