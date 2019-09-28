#pragma once

#include <memory>

#include "external/glad.h"
#include <GLFW/glfw3.h>

namespace ME { namespace Graphics {
    class GameWindow {
        struct windowDeleter {
            void operator()(GLFWwindow* ptr) {
                glfwTerminate();
                glfwDestroyWindow(ptr);
            }
        };
    public:
        GameWindow();
        ~GameWindow();

        bool shouldClose();
        void refresh();
        void run();

    private:
        std::unique_ptr<GLFWwindow, windowDeleter> m_window;
    };
}}