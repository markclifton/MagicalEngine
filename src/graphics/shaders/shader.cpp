#include "shader.h"

#include <iostream>
#include <map>
#include <vector>

#include "glm/gtc/type_ptr.hpp"

#include "utils/file_loader.h"
#include "utils/log.h"

namespace ME { namespace Graphics {
    std::shared_ptr<Shader> load_shader(const std::string &vs, const std::string &fs) {
        static std::map<std::string, std::shared_ptr<Shader>> shaders;
        if (shaders.find(vs) == shaders.end()) {
            shaders[vs] = std::make_shared<Shader>(vs, fs);
        }
        return shaders[vs];
    }

    Shader::Shader(const std::string &vs, const std::string &fs) {
        auto vs_raw = loadFile(vs);
        auto vs_cstr = vs_raw.c_str();
        auto fs_raw = loadFile(fs);
        auto fs_cstr = fs_raw.c_str();

        GLint Compiled;
        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &vs_cstr, nullptr);
        glCompileShader(vert);
        glGetShaderiv(vert, GL_COMPILE_STATUS, &Compiled);
        if (!Compiled) {
            GLint log_length;
            glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &log_length);
            std::vector<char> v(static_cast<size_t>(log_length));
            glGetShaderInfoLog(vert, log_length, nullptr, v.data());
            std::string err(begin(v), end(v));

            glDeleteShader(vert);
            Log<FATAL>() << ("Failed to compile vertex shader: " + err); 
        }

        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fs_cstr, nullptr);
        glCompileShader(frag);
        glGetShaderiv(frag, GL_COMPILE_STATUS, &Compiled);
        if (!Compiled) {
            GLint log_length;
            glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &log_length);
            std::vector<char> v(static_cast<size_t>(log_length));
            glGetShaderInfoLog(frag, log_length, nullptr, v.data());
            std::string err(begin(v), end(v));

            glDeleteShader(vert);
            Log<FATAL>() << ("Failed to compile fragment shader: " + err);
        }

        m_program = glCreateProgram();
        glAttachShader(m_program, vert);
        glAttachShader(m_program, frag);

        glLinkProgram(m_program);

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    Shader::~Shader() {
        if (m_program != 0) glDeleteProgram(m_program);
    }

    void Shader::bind() {
        glUseProgram(m_program);
    }

    int Shader::get_attrib_location(const std::string &attribName) {
        bind();
        return glGetAttribLocation(m_program, attribName.c_str());
    }

    int Shader::get_uniform_location(const std::string &uniformName) {
        bind();
        return glGetUniformLocation(m_program, uniformName.c_str());
    }

    void Shader::set_uniform(const std::string &uniformName, const glm::mat4 &matrix) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }

    void Shader::set_uniform(const std::string &uniformName, const glm::vec3 &vec) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniform3fv(uniform, 1, glm::value_ptr(vec));
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }

    void Shader::set_uniform(const std::string &uniformName, const glm::vec4 &vec) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniform4fv(uniform, 1, glm::value_ptr(vec));
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }

    void Shader::set_uniform(const std::string &uniformName, const int &value) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniform1i(uniform, value);
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }

    void Shader::set_uniform(const std::string &uniformName, const float &value) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniform1f(uniform, value);
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }

    void Shader::set_uniform(const std::string &uniformName, const int &size, const int *data) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniform1iv(uniform, size, data);
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }

    void Shader::set_uniform(const std::string &uniformName, const int &size, const GLuint *data) {
        auto uniform = get_uniform_location(uniformName);
        if (uniform != -1) glUniform1uiv(uniform, size, data);
        else Log<FATAL>() << ("Failed to find uniform: " + uniformName);
    }
}}