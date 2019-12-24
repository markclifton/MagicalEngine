#pragma once 

#include <vector>

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "ecs.h"
#include "graphics/buffer.h"
#include "graphics/shaders/shader.h"

#ifndef OFFSET_MACRO
#define OFFSET_MACRO
#define _offsetof(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif

struct BatchEntity {
    GLuint vao;
    ME::Graphics::VertexBuffer vbo;
    ME::Graphics::IndexBuffer ibo;
    uint32_t numVertices {16 * 16 * 16 * 32};
    uint32_t totalBuffered {0};
    glm::vec3 center;
    bool edited {true};
};

class BatchRenderSystem : public ECS::EntitySystem {
    std::shared_ptr<ME::Graphics::Shader> shader {ME::Graphics::load_shader("vertex", "fragment")};
public:
    BatchRenderSystem() {}
    
    std::vector<BatchEntity*> m_batches;
    int32_t get_batch(glm::vec3 center) {
        m_batches.emplace_back(new BatchEntity());
        glGenVertexArrays( 1, &m_batches.back()->vao );
        m_batches.back()->center = center;
        return m_batches.size() - 1;
    }

    void tick(ECS::World* world, float delta) override {
        shader->bind();
        glm::mat4 p, v;
        ME::Camera::Camera3D::instance().GetMatricies(p, v);
        shader->set_uniform("projection", p*v);

        int index = 0;
        for(auto& cur_batch : m_batches) {
            index++;
			if (!ME::Camera::Camera3D::instance().in_view(cur_batch->center)) {// || !ME::Camera::Camera3D::instance().in_view(cur_batch->center + glm::vec3(16, 0, 16))) {
				cur_batch->vbo.release();
				cur_batch->ibo.release();
				cur_batch->edited = true;
				continue;
			}

            glBindVertexArray( cur_batch->vao );  
            if(cur_batch->edited) {
                cur_batch->vbo.buffer(cur_batch->numVertices * sizeof(VertexComponent), nullptr);
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glEnableVertexAttribArray(3);
                glEnableVertexAttribArray(4);
                glEnableVertexAttribArray(5);
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)_offsetof(VertexComponent, VertexComponent::position));
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)_offsetof(VertexComponent, VertexComponent::color));
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)_offsetof(VertexComponent, VertexComponent::uv));
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(_offsetof(VertexComponent, VertexComponent::xform) + 0 * sizeof(glm::vec4)));
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(_offsetof(VertexComponent, VertexComponent::xform) + 1 * sizeof(glm::vec4)));
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(_offsetof(VertexComponent, VertexComponent::xform) + 2 * sizeof(glm::vec4)));
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexComponent), (void*)(_offsetof(VertexComponent, VertexComponent::xform) + 3 * sizeof(glm::vec4)));

                int currentVerts = 0;
                std::vector<uint32_t> finalIndices;
                world->each<BatchRenderComponent, VisibleComponent, VerticesComponent, IndicesComponent>([&](
                    ECS::Entity* ent, ECS::ComponentHandle<BatchRenderComponent> component, ECS::ComponentHandle<VisibleComponent>, ECS::ComponentHandle<VerticesComponent> verts, ECS::ComponentHandle<IndicesComponent> indices) {
                    if(component.get().batch_id != index - 1) return;

                    cur_batch->vbo.buffer_sub_data(sizeof(VertexComponent) * currentVerts, sizeof(VertexComponent) * verts.get().vertices.size(), &verts.get().vertices[0]);

                    for(auto index : indices.get().indices) finalIndices.push_back(index + currentVerts);
                    currentVerts += verts.get().vertices.size();
                });

                cur_batch->ibo.buffer(sizeof(int32_t) * finalIndices.size(), &finalIndices[0]);
                cur_batch->totalBuffered = finalIndices.size();
                cur_batch->edited = false;
            }

            glDrawElements(GL_TRIANGLES, cur_batch->totalBuffered, GL_UNSIGNED_INT, nullptr);
        }
    }
};