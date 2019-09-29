#pragma once 

#include <map>

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "ecs.h"
#include "graphics/buffers/index_buffer.h"
#include "graphics/buffers/vertex_buffer.h"
#include "graphics/shaders/shader.h"

struct BatchEntity {
    ME::Graphics::IndexBuffer ibo;
    ME::Graphics::VertexBuffer vbo;
    uint32_t numVertices {0};
    uint32_t numIndices {0};
    bool edited {true};
};

class BatchRenderSystem : public ECS::EntitySystem {
    std::shared_ptr<ME::Graphics::Shader> shader {ME::Graphics::load_shader("vertex", "fragment")};
    GLuint m_vao;
public:
    BatchRenderSystem() {
        glGenVertexArrays(1, &m_vao);
    }

    std::map<int32_t, BatchEntity> m_batches;
    int32_t get_batch() {
        //TODO: Support more than 1 batch
        if(m_batches.size() == 0) {
            m_batches[0] = BatchEntity();
        }
        return 0;
    }

    void tick(ECS::World* world, float delta) override {
        glBindVertexArray(m_vao);
        shader->bind();

        for(auto& cur_batch : m_batches) {
            if(cur_batch.second.edited) {
                cur_batch.second.vbo.buffer(cur_batch.second.numVertices * sizeof(VertexComponent), nullptr);
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
                cur_batch.second.vbo.unbind();

                int currentVerts = 0;
                std::vector<uint32_t> finalIndices;
                world->each<BatchRenderComponent>([&](ECS::Entity* ent, ECS::ComponentHandle<BatchRenderComponent> component) {
                    if(component.get().batch_id != cur_batch.first) return;

                    auto& verts = ent->get<VerticesComponent>().get().verts;
                    auto& indices = ent->get<IndicesComponent>().get().indices;

                    cur_batch.second.vbo.buffer_sub_data(sizeof(VertexComponent) * currentVerts , sizeof(VertexComponent) * verts.size(), &verts[0]);

                    for(auto index : indices) finalIndices.push_back(index + currentVerts);
                    currentVerts += verts.size();
                });

                cur_batch.second.ibo.buffer(sizeof(int32_t) * finalIndices.size(), &finalIndices[0]);
                cur_batch.second.ibo.unbind();
                cur_batch.second.edited = false;
            }

            cur_batch.second.ibo.bind();
            glDrawElements(GL_TRIANGLES, cur_batch.second.numIndices, GL_UNSIGNED_INT, nullptr);
        }

        shader->unbind();
        glBindVertexArray(0);
    }
};