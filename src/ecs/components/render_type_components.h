#pragma once

#include <memory>

#include "graphics/buffer.h"

namespace {
    inline void set_bit(int& number, int bit) {
        number = (number |= 1UL << bit);
    }

    inline int check_bit(int number, int bit)  {
        return (number >> bit) & 1UL;
    }
}

struct RenderComponent {};

struct BatchRenderComponent {
    BatchRenderComponent() {}
    BatchRenderComponent(int id) : batch_id(id) {}
    int batch_id;
};

struct InstancedComponent {
    InstancedComponent() {}
    InstancedComponent(ECS::Entity* e) : base(e) {}
    ECS::Entity* base {nullptr};
};

struct InstancedRenderComponent {
    InstancedRenderComponent() {}
    InstancedRenderComponent(ECS::Entity* base) : base(base) {}
    ECS::Entity* base {nullptr};
    ME::Graphics::IndexBuffer* ibo {nullptr};
    ME::Graphics::VertexBuffer* vbo {nullptr};
    ME::Graphics::VertexBuffer* mbo {nullptr};
    std::vector<glm::mat4> transforms;
    bool edited {true};
};

struct VisibleComponent {};