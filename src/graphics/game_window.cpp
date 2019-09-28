#include "game_window.h"

#include "utils/log.h"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include "graphics/shaders/shader.h"

static const struct
{
    float x, y, z;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, -1.f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, -1.f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, -1.f, 0.f, 0.f, 1.f }
};

/*
static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
*/

namespace ME { namespace Graphics {
    GameWindow::GameWindow() {
        /* Initialize the library */
        if (!glfwInit()) Log<FATAL>() << "Failed to init glfw";

        /* Create a windowed mode window and its OpenGL context */
        m_window.reset(glfwCreateWindow(640, 480, "Hello World", NULL, NULL));
        if (!m_window) {
            glfwTerminate();
            Log<FATAL>() << "Failed to create the window";
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window.get());

        /* Load OpenGL pointers, must be don't after context */
        if(!gladLoadGL()) Log<FATAL>() << "Failed to init glad";
    }

    GameWindow::~GameWindow() {
    }

    bool GameWindow::shouldClose() {
        return glfwWindowShouldClose(m_window.get());
    }

    void GameWindow::refresh(){
        glfwPollEvents();
        glfwSwapBuffers(m_window.get());
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GameWindow::run() {

/*
        GLuint vertex_buffer;
        auto s = ME::Graphics::load_shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 3));

        s->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
*/

        while (!shouldClose()) {
            refresh();
        }
    }
}}