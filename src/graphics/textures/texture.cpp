#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "utils/log.h"

namespace ME { namespace Graphics {
    Texture::Texture(const std::string &path)
    {
        int channels;
        unsigned char *data = stbi_load(path.c_str(), &m_width, &m_height, &channels, 0);

        if (data == nullptr) ME::Log<ME::FATAL>() << "Failed to load texture: " + path;

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        if(channels == 3) 
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if(channels == 4) 
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    Texture::~Texture() {
        if (m_textureID != 0) glDeleteTextures(1, &m_textureID);
    }

    void Texture::bind() {
        if (m_textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
        }
    }

    glm::vec2 Texture::get_size() {
        return {m_width, m_height};
    }
}}