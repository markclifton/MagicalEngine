#include "index_buffer.h"

#include "utils/log.h"

namespace ME { namespace Graphics {
    IndexBuffer::IndexBuffer() {
        glGenBuffers(1, &m_ibo);
        if(m_ibo == 0) Log<ME::FATAL>() << "Failed to allocate ibo";
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_ibo);
    }
        
    void IndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    }

    void IndexBuffer::buffer(size_t size, void* indices, int type) {
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, type);
    }

    void IndexBuffer::buffer_sub_data(GLintptr offset, size_t size, void* data) {
        bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    }
}}