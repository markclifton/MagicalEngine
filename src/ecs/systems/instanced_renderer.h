#pragma once 

#include <map>
#include <set>

#include "ecs.h"

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "graphics/buffer.h"
#include "graphics/shaders/shader.h"

//Example usage:
//  auto instancedEntity = create_square(world, glm::vec3(), {step*.95, step*.95});
//  instancedEntity->assign<InstancedRenderComponent>(instancedEntity);
//  auto entity = world->create();
//  entity->assign<InstancedComponent>(instancedEntity);
//  entity->assign<XformComponent>( glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)) );

class InstancedRenderSystem : public ECS::EntitySystem {
    std::shared_ptr<ME::Graphics::Shader> shader {ME::Graphics::load_shader("vertex", "fragment")};
    GLuint m_vao;
public:
    InstancedRenderSystem() {
        glGenVertexArrays(1, &m_vao);
    }

    void tick(ECS::World* world, float delta) override {
        glBindVertexArray(m_vao);
        shader->bind();
        
        std::set<ECS::Entity*> completed;
        world->each<InstancedRenderComponent>([&](ECS::Entity* e1, ECS::ComponentHandle<InstancedRenderComponent> irc) {
            auto currentTarget = irc.get().base;
            if(completed.find(irc.get().base) != completed.end()) return;
            completed.insert(currentTarget);

            auto& vbo = irc.get().vbo;
            auto& ibo = irc.get().ibo;
            auto& mbo = irc.get().mbo;

            //TODO: FIX THIS...
            if(vbo == nullptr) {
                ibo = new ME::Graphics::IndexBuffer();
                vbo = new ME::Graphics::VertexBuffer();
                mbo = new ME::Graphics::VertexBuffer();
            }

            auto& transforms = irc.get().transforms;

            auto& verts = e1->get<VerticesComponent>().get().verts;
            auto& indices = e1->get<IndicesComponent>().get().indices;

            if(irc.get().edited) {
                vbo->buffer(sizeof(VertexComponent) * verts.size(), &verts[0]);
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)offsetof(VertexComponent, VertexComponent::position));
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)offsetof(VertexComponent, VertexComponent::color));
                vbo->unbind();
                
                ibo->buffer(sizeof(int32_t) * indices.size(), &indices[0]);
                ibo->unbind();

                transforms.clear();
                world->each<InstancedComponent>([&](ECS::Entity* e2, ECS::ComponentHandle<InstancedComponent> ic) {
                    auto newTarget = ic.get().base;
                    if(newTarget != currentTarget) return;
                    transforms.push_back(e2->get<XformComponent>().get().xform);
                });

                mbo->buffer(transforms.size() * static_cast<long>(sizeof(glm::mat4)), &transforms[0], GL_STATIC_DRAW);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(0 * sizeof(glm::vec4)));
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(1 * sizeof(glm::vec4)));
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(2 * sizeof(glm::vec4)));
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(3 * sizeof(glm::vec4)));
                glVertexAttribDivisor(2, 1);
                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                mbo->unbind();

                irc.get().edited = false;
            }

            glm::mat4 p, v;
            ME::Camera::Camera3D::instance().GetMatricies(p, v);
            shader->set_uniform("projection", p*v);

            ibo->bind();
            glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, transforms.size());
        });
        shader->unbind();

        glBindVertexArray(0);
    }
};