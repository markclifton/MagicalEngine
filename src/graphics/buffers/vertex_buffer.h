#pragma once

#include "glad.h"

namespace ME { namespace Graphics {
    class VertexBuffer {
    public:
        VertexBuffer();
        ~VertexBuffer();
        
        void bind();
        void buffer(size_t size, void* verts, int drawType = GL_STATIC_DRAW);
        void buffer_sub_data(GLintptr offset, size_t size, void* data);

    private:
        GLuint m_vbo;
    };
}}