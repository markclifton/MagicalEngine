#include "window.h"

#include "input/keyhandler.h"
#include "input/mousehandler.h"
#include "utils/log.h"

namespace {
    void window_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void key_forwarder(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto w = static_cast<ME::Game::Window*>(glfwGetWindowUserPointer(window));
        w->keyHandler(key, scancode, action, mods);
    }

    void cursor_forwarder(GLFWwindow* window, double xpos, double ypos) {
        auto w = static_cast<ME::Game::Window*>(glfwGetWindowUserPointer(window));
        w->mouseHandler(xpos,ypos);
    }

    void mouse_forwarder(GLFWwindow* window, int button, int action, int mods) {
        auto w = static_cast<ME::Game::Window*>(glfwGetWindowUserPointer(window));
        w->mouseHandler(button, action, mods);
    }
}

namespace ME { namespace Game {
    Window::Window() {
        if (!glfwInit()) Log<FATAL>() << "Failed to init glfw";
        m_window.reset(glfwCreateWindow(1280, 720, "Hello World", nullptr, nullptr));
        if (!m_window) {
            glfwTerminate();
            Log<FATAL>() << "Failed to create the window";
        }
        glfwMakeContextCurrent(m_window.get());
        if(!gladLoadGL()) Log<FATAL>() << "Failed to init glad";

        glfwSetWindowSizeCallback(m_window.get(), window_size_callback);
        glfwSetWindowUserPointer(m_window.get(), this);
        glfwSetKeyCallback(m_window.get(), key_forwarder);
        glfwSetCursorPosCallback(m_window.get(), cursor_forwarder);
        glfwSetMouseButtonCallback(m_window.get(), mouse_forwarder);
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(m_window.get());
    }

    void Window::update(){
        glfwPollEvents();
        glfwSwapBuffers(m_window.get());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Window::registerHandler(Input::KeyHandler* handler) {
        m_keyHandlers.push_back(handler);
    }

    void Window::registerHandler(Input::MouseHandler* handler) {
        m_mouseHandlers.push_back(handler);
    }

    void Window::keyHandler(int key, int scancode, int action, int mods) {
        if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if(key == GLFW_KEY_F2 && action == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else if(key == GLFW_KEY_F3 && action == GLFW_PRESS) {
            glEnable(GL_CULL_FACE);
        } else if(key == GLFW_KEY_F4 && action == GLFW_PRESS) {
            glDisable(GL_CULL_FACE);
        }

        for(auto& handler : m_keyHandlers) {
            handler->process(key, scancode, action, mods);
        }
    }

    void Window::mouseHandler(double xpos, double ypos) {
        for(auto& handler : m_mouseHandlers) {
            handler->process(xpos, ypos);
        }
    }

    void Window::mouseHandler(int button, int action, int mods) {
        for(auto& handler : m_mouseHandlers) {
            handler->process(button, action, mods);
        }
    }

}}