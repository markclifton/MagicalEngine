#pragma once

#include <memory>

#include "external/glad.h"
#include <GLFW/glfw3.h>

namespace ME { namespace Game {
    class Window {
        struct windowDeleter {
            void operator()(GLFWwindow* ptr) {
                glfwTerminate();
                glfwDestroyWindow(ptr);
            }
        };
    public:
        Window();
        ~Window();

        bool shouldClose();
        void update();

    private:
        std::unique_ptr<GLFWwindow, windowDeleter> m_window;
    };
}}