#pragma once 

#include "ecs.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "graphics/textures/texture_manager.h"

struct RawComponents {
    RawComponents(VerticesComponent v, IndicesComponent i) : vertices(v), indices(i) {}
    VerticesComponent vertices;
    IndicesComponent indices;
};

inline RawComponents create_triangle(glm::vec3 position) {
    return {
        VerticesComponent({
            {glm::vec3(-.6, -1, -1), glm::vec3(1, 0, 0), glm::mat4(1.f) * glm::translate(position)},
            {glm::vec3(.6, -1, -1), glm::vec3(0, 1, 0), glm::mat4(1.f) * glm::translate(position)},
            {glm::vec3(.0, 0, -1), glm::vec3(0, 0, 1), glm::mat4(1.f) * glm::translate(position)}
        }),
        IndicesComponent({
            1,
            2,
            0
        })
    };
}

inline RawComponents create_square(const std::string& texture, glm::mat4 xform, int repeatX = 1, int repeatY = 1) {
    double textureHandle = -1;
    if(texture != "") textureHandle = ME::Graphics::TextureManager::instance().load_texture(texture);
    return {
        VerticesComponent({
            {glm::vec3(0,0,0), glm::vec3(1,1,1), glm::vec3(0.f, repeatY, textureHandle), xform},
            {glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec3(repeatX, repeatY, textureHandle), xform},
            {glm::vec3(1,1,0), glm::vec3(1,1,1), glm::vec3(repeatX, 0.f, textureHandle), xform},
            {glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec3(0.f, 0.f, textureHandle), xform}
        }),
        IndicesComponent({
            0,
            1,
            2,
            0,
            2,
            3
        })
    };
}

inline RawComponents create_square(const std::string& texture, glm::vec3 position) {
    return create_square(texture, glm::translate(glm::mat4(1.f), position));
}