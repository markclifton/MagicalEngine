#pragma once

#include <map>

#include "glad.h"

#include "texture.h"
#include "utils/non_copyable.h"
#include "utils/log.h"

#define MAX_TEXTURES 1024

//TODO: Thread-safe.
namespace ME { namespace Graphics {
    class TextureManager : NonCopyable {
    public:
    static TextureManager& instance() {
        static TextureManager mgr;
        return mgr;
    }

    uint64_t load_texture(const std::string& file) {
        if(m_textures.size() > 1000) Log<FATAL>() << "Exceeded max textures";

        if(m_textures.find(file) == m_textures.end()) {
            m_textures.emplace(file, "res/textures/" + file);
        }

        //TODO: Fix this.
        auto& texture = m_textures.find(file)->second;
        unsigned int texId = texture.get_id();
        if(m_handleMap.find(texId) == m_handleMap.end()) {
            uint64_t nextHandle = m_textures.size() - 1;
            m_handleMap[texId] = nextHandle;
            m_handles[nextHandle] = glGetTextureHandleNV(texId);
            glMakeTextureHandleResidentNV(m_handles[nextHandle]);
        }

        return m_handleMap[texId];
    }

    void bind_all_textures() {
        unsigned int container;
        glGenBuffers(1, &container); 
        glBindBuffer(GL_UNIFORM_BUFFER, container);
        glBufferData(GL_UNIFORM_BUFFER, m_textures.size() * sizeof(uint64_t), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, container, 0, m_textures.size() * sizeof(uint64_t));
        glBufferSubData(GL_UNIFORM_BUFFER, 0, m_textures.size() * sizeof(uint64_t), m_handles);
    }

    private:        
        TextureManager() {
            for(int i=0; i<MAX_TEXTURES; ++i) {
                m_handles[i] = 0;
            }
        }

        std::map<std::string, ME::Graphics::Texture> m_textures;
        std::map<uint64_t, uint64_t> m_handleMap;
        uint64_t m_handles [MAX_TEXTURES];
    };
}}