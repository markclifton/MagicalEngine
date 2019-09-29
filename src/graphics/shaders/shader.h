#pragma once

#include <memory>
#include <string>

#include "glad.h"
#include "glm/glm.hpp"

namespace ME { namespace Graphics {
    class  Shader
    {
    public:
        Shader() = default;
        Shader(const std::string &vs, const std::string &fs);
        ~Shader();

        void bind();
        void unbind();

        int get_attrib_location(const std::string &attribName);
        int get_uniform_location(const std::string &uniformName);

        void set_uniform(const std::string &uniformName, const glm::mat4 &matrix);
        void set_uniform(const std::string &uniformName, const glm::vec3 &vec);
        void set_uniform(const std::string &uniformName, const glm::vec4 &vec);
        void set_uniform(const std::string &uniformName, const int &value);
        void set_uniform(const std::string &uniformName, const float &value);
        void set_uniform(const std::string &uniformName, const int &size, const int *data);
        void set_uniform(const std::string &uniformName, const int &size, const GLuint *data);

    private:
        GLuint m_program;
    };

    std::shared_ptr<Shader> load_shader(const std::string &vs, const std::string &fs);
}}