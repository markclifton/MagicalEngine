#pragma once

#include "ecs.h"

#include "glm/glm.hpp"

struct VertexComponent {
    VertexComponent(glm::vec3 p, glm::vec3 c) : position(p), color(c) {}
    glm::vec3 position {glm::vec3(0)};
    glm::vec3 color {glm::vec3(0)};
};

struct VerticesComponent {
    VerticesComponent(std::vector<VertexComponent> v) : verts(v) {}
    VerticesComponent() {}
    std::vector<VertexComponent> verts {};
};

struct IndicesComponent {
    IndicesComponent(std::vector<uint32_t> i) : indices(i) {}
    IndicesComponent() {}
    std::vector<uint32_t> indices {};
};

struct XformComponent {
    XformComponent() {}
    XformComponent(glm::mat4 x) : xform(x) {}
    glm::mat4 xform;
};