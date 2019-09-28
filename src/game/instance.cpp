#include "instance.h"

#include "ecs/components/basic_shapes.h"
#include "ecs/systems/renderer.h"
#include "ecs/systems/batch_renderer.h"
#include "ecs.h"
#include "graphics/shaders/shader.h"
#include "utils/log.h"
#include "utils/timer.h"
namespace ME { namespace Game {
    Instance::Instance() 
    : m_window(std::make_unique<Window>())
    {}

    void Instance::run() {
        load();

        // TODO: Push this logic down into "levels"
        auto s = ME::Graphics::load_shader("vertex", "fragment");
        s->bind();

        auto renderSystem = new RenderSystem();
        auto batchSystem = new BatchRenderSystem();

        auto world = ECS::World::createWorld();
        world->registerSystem(renderSystem);
        world->registerSystem(batchSystem);

        auto step = .0625f * .25f;
        int totalItems = 0;
        for(float x = -2.f; x <= 2.f; x+=step){
            for(float y = -2.f; y <= 2.f; y+=step){
                auto entity = create_square(world, glm::vec3(x, y, 0.f), {step*.95, step*.95});
                //entity->assign<RenderComponent>(); // <- Horrible performance
                entity->assign<BatchRenderComponent>( batchSystem->get_batch() ); // <- Decent performance
                batchSystem->m_batches[0].numVertices += 4;
                batchSystem->m_batches[0].numIndices += 6;
                batchSystem->m_batches[0].edited = true;
                totalItems++;
            }
        }

       // glfwSwapInterval(0);

       // ME::Timer t;
        while(!m_window->shouldClose()) {
            update();
            s->bind();
            world->tick(glfwGetTime());
         //   ME::Log<ME::DEBUG>() << ("Frame time: " + std::to_string(t.reset()) + " items: " + std::to_string(totalItems));
        }
    }

    void Instance::load() {
    }

    void Instance::update() {
        m_window->update();
    }
}}