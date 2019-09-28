#pragma once

#include "glad.h"

namespace ME { namespace Graphics {
    class VertexBuffer {
    public:
        VertexBuffer();
        ~VertexBuffer();
        
        void bind();
        void buffer(size_t size, void* verts, int drawType = GL_STATIC_DRAW);

    private:
        GLuint m_vbo;
    };
}}