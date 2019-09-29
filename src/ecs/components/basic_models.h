#pragma once 

#include "ecs.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"

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
        {0}, {6}, {4},
        {0}, {2}, {6},
        {0}, {3}, {2},
        {0}, {1}, {3},
        {2}, {7}, {6},
        {2}, {3}, {7},
        {4}, {6}, {7},
        {4}, {7}, {5},
        {0}, {4}, {5},
        {0}, {5}, {1},
        {1}, {5}, {7},
        {1}, {7}, {3}
    });
    entity->assign<IndicesComponent>(indices);

    return entity;
}