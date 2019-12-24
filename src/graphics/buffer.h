#pragma once

#include "utils/log.h"

namespace ME { namespace Graphics {
    enum BUFFER {
        VBO,
        IBO
    };

    template <typename EnumClass, EnumClass EnumVal>
    class BufferImpl {
    public:
        BufferImpl(int type) : m_type(type), m_created(true) {
            glGenBuffers(1, &m_buffer);
            if(m_buffer == 0) 
                Log<ME::FATAL>() << "Failed to allocate buffer";
        }

        ~BufferImpl() {
            glDeleteBuffers(1, &m_buffer);
        }

        void bind() {
			if (!m_created) {
				glGenBuffers(1, &m_buffer);
				m_created = true;
			}
            glBindBuffer(m_type, m_buffer);
        }

        void unbind() {
            glBindBuffer(m_type, 0);
        }

        void buffer(size_t size, void* verts, int drawType = GL_STATIC_DRAW ) {
            bind();
            glBufferData(m_type, size, verts, drawType);
        }

        void buffer_sub_data(GLintptr offset, size_t size, void* data){
            bind();
            glBufferSubData(m_type, offset, size, data);
        }

		void release() {
			if (m_created) {
				m_created = false;
				glDeleteBuffers(1, &m_buffer);
			}
		}

    private:
        GLuint m_buffer;
        int m_type;
        int m_drawType;
		bool m_created;
    };

    class VertexBuffer : public BufferImpl<decltype(BUFFER::VBO), VBO> {
    public:
        VertexBuffer() :  BufferImpl<decltype(BUFFER::VBO), VBO>(GL_ARRAY_BUFFER) {}
    };

    class IndexBuffer : public BufferImpl<decltype(BUFFER::IBO), IBO> {
    public:
        IndexBuffer() :  BufferImpl<decltype(BUFFER::IBO), IBO>(GL_ELEMENT_ARRAY_BUFFER) {}
    };

}}
