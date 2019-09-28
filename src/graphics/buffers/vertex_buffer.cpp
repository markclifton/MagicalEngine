#include "vertex_buffer.h"

#include "utils/log.h"

#include "ecs/components/renderable_component.h"

namespace ME { namespace Graphics {
        VertexBuffer::VertexBuffer() {
            glGenBuffers(1, &m_vbo);
            if(m_vbo == 0) Log<ME::FATAL>() << "Failed to allocate vbo";
        }

        VertexBuffer::~VertexBuffer() {
            glDeleteBuffers(1, &m_vbo);
        }   

        void VertexBuffer::bind() {
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

            //TODO: Move to better location
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*) 0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*) (sizeof(float) * 3));
        }

        void VertexBuffer::buffer(size_t size, void* verts, int drawType) {
            bind();
            glBufferData(GL_ARRAY_BUFFER, size, verts, drawType);
        }
}}