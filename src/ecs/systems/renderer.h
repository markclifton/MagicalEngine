#pragma once 

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "ecs.h"
#include "graphics/buffer.h"
#include "graphics/shaders/shader.h"

class RenderSystem : public ECS::EntitySystem {
public:
    RenderSystem(){
    }

    ME::Graphics::IndexBuffer ibo;
    ME::Graphics::VertexBuffer vbo;

    std::shared_ptr<ME::Graphics::Shader> shader {ME::Graphics::load_shader("vertex", "fragment")};

    void tick(ECS::World* world, float delta) override {
        shader->bind();
        
        world->each<RenderComponent>([&](ECS::Entity* ent, ECS::ComponentHandle<RenderComponent>) {
            auto& verts = ent->get<VerticesComponent>().get().verts;
            vbo.buffer(sizeof(VertexComponent) * verts.size(), &verts[0]);

            auto& indices = ent->get<IndicesComponent>().get().indices;
            ibo.buffer(sizeof(int32_t) * indices.size(), &indices[0]);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
            glEnableVertexAttribArray(5);
            
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)offsetof(VertexComponent, VertexComponent::position));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)offsetof(VertexComponent, VertexComponent::color));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(offsetof(VertexComponent, VertexComponent::xform) + 0 * sizeof(glm::vec4)));
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(offsetof(VertexComponent, VertexComponent::xform) + 1 * sizeof(glm::vec4)));
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(offsetof(VertexComponent, VertexComponent::xform) + 2 * sizeof(glm::vec4)));
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(offsetof(VertexComponent, VertexComponent::xform) + 3 * sizeof(glm::vec4)));
            
            glm::mat4 p, v;
            ME::Camera::Camera3D::instance().GetMatricies(p, v);
            shader->set_uniform("projection", p*v);

            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

            shader->unbind();
        });
    }
};