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

        inline GLuint get_id() { return m_textureID; }
        inline glm::vec2 get_size() { return {m_width, m_height}; }
        
    private:
        GLuint m_textureID {0};
        int m_width {0};
        int m_height {0};
    };
}}