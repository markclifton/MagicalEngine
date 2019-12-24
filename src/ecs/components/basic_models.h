#pragma once 

#include "ecs.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ecs/components/basic_shapes.h"
#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"

glm::mat4 get_transform(glm::vec3 position, float deg, glm::vec3 axis, glm::vec3 scale) {
    return glm::translate(glm::mat4(1), position) * glm::rotate(glm::mat4(1), deg, axis) * glm::scale(glm::mat4(1), scale);
}

enum SIDES {
    FRONT = 0,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

inline void append_vertices(VerticesComponent& dest, VerticesComponent& src) {
    dest.vertices.insert(dest.vertices.end(), src.vertices.begin(), src.vertices.end());
}

inline void append_indices(IndicesComponent& dest, IndicesComponent& src, int numVerts) {
    for(auto indice : src.indices) dest.indices.push_back(numVerts + indice);
}

inline void add_side(VerticesComponent& vDest, IndicesComponent& iDest, const std::string& texture, glm::mat4 xform, int x = 1, int y = 1) {
    auto side = create_square(texture, xform, x, y);
    append_indices(iDest, side.indices, vDest.vertices.size());
    append_vertices(vDest, side.vertices);
}

inline ECS::Entity* create_cube(ECS::World* world, const std::string& texture, glm::vec3 position, int hiddenSides = 0, glm::vec3 scale = {1, 1, 1} ) {
    auto entity = world->create();

    VerticesComponent vertices;
    IndicesComponent indices;

    float xMod=1, yMod=1, zMod=1;
    if(scale.x > 0) {
        xMod = scale.x;
    }if(scale.y > 0) {
        yMod = scale.y;
    }if(scale.z > 0) {
        zMod = scale.z;
    }

    auto sideTexture = check_bit(hiddenSides, TOP) ? "dirt.jpg" : "grass_side.jpg";

    if(!check_bit(hiddenSides, FRONT)) add_side(vertices, indices, sideTexture, get_transform(position, 0, {0,1,0}, {scale.x, scale.y, 1}), scale.x, scale.y);
    if(!check_bit(hiddenSides, BACK)) add_side(vertices, indices, sideTexture, get_transform(position + glm::vec3(xMod,0,-zMod), glm::pi<float>(), {0,1,0}, {scale.x, scale.y, 1}), scale.x, scale.y);
    if(!check_bit(hiddenSides, LEFT)) add_side(vertices, indices, sideTexture, get_transform(position + glm::vec3(0,0,-zMod), -glm::pi<float>()/2.f, {0,1,0}, {scale.z, scale.y, 1}), scale.z, scale.y);
    if(!check_bit(hiddenSides, RIGHT)) add_side(vertices, indices, sideTexture, get_transform(position + glm::vec3(xMod,0,0), glm::pi<float>()/2.f, {0,1,0}, {scale.z, scale.y, 1}), scale.z, scale.y);
    if(!check_bit(hiddenSides, TOP)) add_side(vertices, indices, "grass.png", get_transform(position + glm::vec3(0,yMod,0), -glm::pi<float>()/2.f, {1,0,0}, {scale.x, scale.z, 1}), scale.x, scale.z);
    if(!check_bit(hiddenSides, BOTTOM)) add_side(vertices, indices, "dirt.jpg", get_transform(position + glm::vec3(0,0,-zMod), glm::pi<float>()/2.f, {1,0,0}, {scale.x, scale.z, 1}), scale.x, scale.z);

    entity->assign<VerticesComponent>(vertices);
    entity->assign<IndicesComponent>(indices);

    return entity;
}

inline ECS::Entity* create_cube(ECS::World* world, glm::vec3 position, glm::vec2 size = {.25f, .25f}) {
    auto entity = world->create();

    size *= .5f;
    VerticesComponent vertices ({
        {glm::vec3(0, 0, 0), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(0, 0, 1), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(0, 1, 0), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(0, 1, 1), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(1, 0, 0), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(1, 0, 1), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(1, 1, 0), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)},
        {glm::vec3(1, 1, 1), glm::vec3(1.f, 1.f, 1.f), glm::translate(glm::mat4(1.f), position)}
    });
    entity->assign<VerticesComponent>(vertices);

    IndicesComponent indices ({
        0, 6, 4,
        0, 2, 6,
        0, 3, 2,
        0, 1, 3,
        2, 7, 6,
        2, 3, 7,
        4, 6, 7,
        4, 7, 5,
        0, 4, 5,
        0, 5, 1,
        1, 5, 7,
        1, 7, 3
    });
    entity->assign<IndicesComponent>(indices);

    return entity;
}

