#pragma once 

#include "ecs.h"

#include "glm/glm.hpp"
#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"

//TODO: Stop manually xforming vertices, use the xforms

inline ECS::Entity* create_triangle(ECS::World* world, glm::vec3 position) {
    auto entity = world->create();

    VerticesComponent vertices ({
        {position + glm::vec3(-.6f, -.4f, -1.f), glm::vec3(1.f, 0.f, 0.f)},
        {position + glm::vec3(.6f, -.4f, -1.f), glm::vec3(0.f, 1.f, 0.f)},
        {position + glm::vec3(.0f, .6f, -1.f), glm::vec3(0.f, 0.f, 1.f)}
    });
    entity->assign<VerticesComponent>(vertices);

    IndicesComponent indices ({
        {1},
        {2},
        {0}
    });
    entity->assign<IndicesComponent>(indices);

    return entity;
}

inline ECS::Entity* create_square(ECS::World* world, glm::vec3 position, glm::vec2 size = {.25f, .25f}) {
    auto entity = world->create();

    size *= .5f;
    VerticesComponent vertices ({
        {position + glm::vec3(-size.x, -size.y, -1.f), glm::vec3(1.f, 0.f, 0.f)},
        {position + glm::vec3(size.x, -size.y, -1.f), glm::vec3(0.f, 1.f, 0.f)},
        {position + glm::vec3(size.x, size.y, -1.f), glm::vec3(0.f, 0.f, 1.f)},
        {position + glm::vec3(-size.x, size.y, -1.f), glm::vec3(1.f, 0.f, 1.f)}
    });
    entity->assign<VerticesComponent>(vertices);

    IndicesComponent indices ({
        {0},
        {1},
        {2},
        {0},
        {2},
        {3}
    });
    entity->assign<IndicesComponent>(indices);

    return entity;
}