#pragma once

#include "glad.h"

namespace ME { namespace Graphics {
    class IndexBuffer {
    public:
        IndexBuffer();
        ~IndexBuffer();
        
        void bind();
        void buffer(size_t size, void* indices, int type = GL_STATIC_DRAW);

    private:
        GLuint m_ibo;
    };
}}