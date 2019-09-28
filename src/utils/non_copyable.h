#pragma once

namespace ME {
    class NonCopyable {
    public:
        NonCopyable() = default;
        NonCopyable(NonCopyable const&) = delete;
        void operator=(NonCopyable const&) = delete;
    };
}