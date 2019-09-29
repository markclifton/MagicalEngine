#pragma once

#include <string>

#include "glad.h"
#include "glm/glm.hpp"

#include "utils/non_copyable.h"

namespace ME { namespace Graphics {
    class Texture : NonCopyable {
    public:
        Texture(const std::string& path);
        ~Texture();

        void bind();
        glm::vec2 get_size();

    private:
        GLuint m_textureID {0};
        int m_width {0};
        int m_height {0};

    };
}}