#include "vertex_buffer.h"

#include "utils/log.h"

#include "ecs/components/renderable_component.h"

namespace ME { namespace Graphics {
        VertexBuffer::VertexBuffer() {
            glGenBuffers(1, &m_vbo);
            if(m_vbo == 0) Log<ME::FATAL>() << "Failed to allocate vbo";
            Log<ME::DEBUG>() << "VBO Created: " + std::to_string(m_vbo);
        }

        VertexBuffer::~VertexBuffer() {
            glDeleteBuffers(1, &m_vbo);
        }   

        void VertexBuffer::bind() {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        }

        void VertexBuffer::unbind() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void VertexBuffer::buffer(size_t size, void* verts, int drawType) {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, size, verts, drawType);
        }

        void VertexBuffer::buffer_sub_data(GLintptr offset, size_t size, void* data) {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        }
}}