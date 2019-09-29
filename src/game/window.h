#pragma once

#include <memory>
#include <vector>

#include "external/glad.h"
#include <GLFW/glfw3.h>

namespace ME { namespace Input {
    class KeyHandler;
    class MouseHandler;
}}

namespace ME { namespace Game {
    class Window {
    public:
        Window();

        bool shouldClose();
        void update();
        
        //Only ecs systems should register
        void registerHandler(Input::KeyHandler* handler);
        void registerHandler(Input::MouseHandler* handler);

        //Do not call directly
        void keyHandler(int key, int scancode, int action, int mods);
        void mouseHandler(double xpos, double ypos);
        void mouseHandler(int button, int action, int mods);

    private:
        std::vector<Input::KeyHandler*> m_keyHandlers;
        std::vector<Input::MouseHandler*> m_mouseHandlers;

        struct WindowDeleter {
            void operator()(GLFWwindow* ptr) {
                glfwTerminate();
                glfwDestroyWindow(ptr);
            }
        };
        std::unique_ptr<GLFWwindow, WindowDeleter> m_window;
    };
}}