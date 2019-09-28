#pragma once

#include "glad.h"

namespace ME { namespace Graphics {
    class IndexBuffer {
    public:
        IndexBuffer();
        ~IndexBuffer();
        
        void bind();
        void buffer(size_t size, void* indices, int type = GL_STATIC_DRAW);
        void buffer_sub_data(GLintptr offset, size_t size, void* data);

    private:
        GLuint m_ibo;
    };
}}