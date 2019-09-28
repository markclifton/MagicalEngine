#include "window.h"

#include "utils/log.h"

namespace {
    void window_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}

namespace ME { namespace Game {
    Window::Window() {
        if (!glfwInit()) Log<FATAL>() << "Failed to init glfw";
        m_window.reset(glfwCreateWindow(640, 480, "Hello World", NULL, NULL));
        if (!m_window) {
            glfwTerminate();
            Log<FATAL>() << "Failed to create the window";
        }
        glfwMakeContextCurrent(m_window.get());
        if(!gladLoadGL()) Log<FATAL>() << "Failed to init glad";

        glfwSetWindowSizeCallback(m_window.get(), window_size_callback);
    }

    Window::~Window() {}

    bool Window::shouldClose() {
        return glfwWindowShouldClose(m_window.get());
    }

    void Window::update(){
        glfwPollEvents();
        glfwSwapBuffers(m_window.get());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}}