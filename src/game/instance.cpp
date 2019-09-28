#include "instance.h"

#include "ecs/components/basic_shapes.h"
#include "ecs/ecs.h"
#include "graphics/shaders/shader.h"
#include "utils/log.h"

#include "graphics/buffers/index_buffer.h"
#include "graphics/buffers/vertex_buffer.h"
class BasicRenderSystem : public ECS::EntitySystem {
public:
    BasicRenderSystem(){
    }

    ME::Graphics::IndexBuffer ibo;
    ME::Graphics::VertexBuffer vbo;

    void tick(ECS::World* world, float delta) override {
        world->each<VerticesComponent>([&](ECS::Entity* ent, ECS::ComponentHandle<VerticesComponent> v) {
            //Brute force test code
            auto& verts = v.get().verts;
            vbo.buffer(sizeof(VertexComponent) * verts.size(), &verts[0]);

            auto& indices = ent->get<IndicesComponent>().get().indices;
            ibo.buffer(sizeof(int32_t) * indices.size(), &indices[0]);
            
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        });
    }
};

namespace ME { namespace Game {
    Instance::Instance() 
    : m_window(std::make_unique<Window>())
    {}

    void Instance::run() {
        load();

        // TODO: Push this logic down into "levels"
        auto s = ME::Graphics::load_shader("vertex", "fragment");
        s->bind();

        auto world = ECS::World::createWorld();
        world->registerSystem(new BasicRenderSystem());

        //create_triangle(world, glm::vec3());

        for(float x = -1.f; x <= 1.f; x+=.0625f){
            for(float y = -1.f; y <= 1.f; y+=.0625f){
                create_square(world, glm::vec3(x, y, 0.f), {.0623f, .0623f});
            }
        }

        while(!m_window->shouldClose()) {
            update();

            s->bind();
            world->tick(glfwGetTime());
        }
    }

    void Instance::load() {
        //glGenVertexArrays(1, &m_vao);
        //glBindVertexArray(m_vao);
    }

    void Instance::update() {
        m_window->update();
    }
}}