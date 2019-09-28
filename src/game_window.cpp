#include "game_window.h"

#include "utils/log.h"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

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

namespace ME {
    GameWindow::GameWindow() {
        /* Initialize the library */
        if (!glfwInit()) FLog::instance() << "Failed to init glfw";

        /* Create a windowed mode window and its OpenGL context */
        m_window.reset(glfwCreateWindow(640, 480, "Hello World", NULL, NULL));
        if (!m_window)
        {
            glfwTerminate();
            FLog::instance() << "Failed to create the window";
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window.get());

        /* Load OpenGL pointers, must be don't after context */
        if(!gladLoadGL()) FLog::instance() << "Failed to init glad";
    }

    GameWindow::~GameWindow() {
    }

    void GameWindow::run() {

        GLuint vertex_buffer, vertex_shader, fragment_shader, program;
        GLint mvp_location, vpos_location, vcol_location;

        glfwSwapInterval(1);

        glGenBuffers(1, &vertex_buffer);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        mvp_location = glGetUniformLocation(program, "MVP");
        vpos_location = glGetAttribLocation(program, "vPos");
        vcol_location = glGetAttribLocation(program, "vCol");
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                            sizeof(vertices[0]), (void*) 0);
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                            sizeof(vertices[0]), (void*) (sizeof(float) * 2));
        while (!glfwWindowShouldClose(m_window.get()))
        {
            glm::mat4 mvp;
            glClear(GL_COLOR_BUFFER_BIT);
            mvp = glm::mat4(1.f);
            glUseProgram(program);
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glfwSwapBuffers(m_window.get());
            glfwPollEvents();
        }
    }

}
