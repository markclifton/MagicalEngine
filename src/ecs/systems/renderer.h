#pragma once 

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "ecs.h"
#include "graphics/buffers/index_buffer.h"
#include "graphics/buffers/vertex_buffer.h"

class RenderSystem : public ECS::EntitySystem {
public:
    RenderSystem(){
    }

    ME::Graphics::IndexBuffer ibo;
    ME::Graphics::VertexBuffer vbo;

    void tick(ECS::World* world, float delta) override {
        world->each<RenderComponent>([&](ECS::Entity* ent, ECS::ComponentHandle<RenderComponent>) {
            //Brute force test code
            auto& verts = ent->get<VerticesComponent>().get().verts;
            vbo.buffer(sizeof(VertexComponent) * verts.size(), &verts[0]);

            auto& indices = ent->get<IndicesComponent>().get().indices;
            ibo.buffer(sizeof(int32_t) * indices.size(), &indices[0]);
            
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        });
    }
};