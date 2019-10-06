#include "instance.h"

#include "camera/camera.h"
#include "ecs/components/basic_models.h"
#include "ecs/components/basic_shapes.h"
#include "ecs/systems/renderer.h"
#include "ecs/systems/batch_renderer.h"
#include "ecs/systems/instanced_renderer.h"
#include "ecs.h"
#include "graphics/shaders/shader.h"
#include "utils/log.h"
#include "utils/timer.h"

//TEST
#include "ecs/components/renderable_component.h"
#include "graphics/textures/texture.h"
#include "graphics/mesh.h"
#include "graphics/textures/texture_manager.h"

#include "ecs/systems/chunk_system.h"

namespace ME { namespace Game {
    Instance::Instance() 
    : m_window(std::make_unique<Window>())
    {}

    void Instance::run() {
        load();

        auto renderSystem = new RenderSystem();
        auto batchSystem = new BatchRenderSystem();
        auto instancedSystem = new InstancedRenderSystem();

        auto world = ECS::World::createWorld();
        world->registerSystem(batchSystem);
        world->registerSystem(instancedSystem);
        world->registerSystem(renderSystem);


        batchSystem->get_batch();
        auto chunkSystem = new ChunkSystem(world);
        chunkSystem->generate_chunk(0, 0);

        batchSystem->m_batches[0].numVertices = 16 * 16 * 16 * 32;

        for(int i=0; i<-1; i+=3) {
            auto entity = create_cube(world, "dirt_side.jpg", glm::vec3(i,0,1), 0, glm::vec3(16,16,16));
            entity->assign<BatchRenderComponent>( 0 );
            entity->assign<VisibleComponent>();
            
            //auto entity2 = create_cube(world, "dirt_side.jpg", glm::vec3(i,0,2), 0, glm::vec3(16,1,1));
            //entity2->assign<BatchRenderComponent>( 0 );
            //entity2->assign<VisibleComponent>();
        }
/*
        std::vector<Chunk> chunks;
        int numChunks = 1;
        int chunkSize = 16;
        for(int v=0; v<numChunks; v++){ for(int w=0; w<numChunks; w++){
            chunks.emplace_back(world, chunkSize*v , chunkSize*w);
            for(float x = 0; x < chunkSize; x+=1){ for(float y = 0; y < chunkSize; y+=1){ for(float z = 0; z < chunkSize; z+=1){
                auto entity = create_cube(world, "dirt.png", glm::vec3(x-chunkSize*v, y, z-chunkSize*w), 0);
                entity->assign<BatchRenderComponent>( batchSystem->get_batch() );
                chunks.back().add_block(entity, x, y, z); //This should improve performance...
            } } }
            chunks.back().resolve_visibility();
        } }
        batchSystem->m_batches[0].numVertices = chunkSize * chunkSize * chunkSize * 36;
*/
        //generate_chunk(world);

        ME::Camera::Camera3D::instance().SetViewport(0, 0, 1280, 720);
        m_window->registerHandler(&ME::Camera::Camera3D::instance());

        //glfwSwapInterval(0);
        Timer timer;
        int counter = 0;

        double lastUpdate = glfwGetTime();
        while(!m_window->shouldClose()) {
            update();

            world->tick( glfwGetTime() - lastUpdate );
            lastUpdate = glfwGetTime();

            counter++;
            if(timer.get() > 1000){
                ME::Log<ME::DEBUG>() << "Frame Time: " + std::to_string(std::round(counter / timer.reset() * 1000.));
                counter = 0;
            }
        }
    }

    void Instance::load() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(.2, .3, .8, 1.f);
    }

    void Instance::update() {
        m_window->update();
        Camera::Camera3D::instance().Update();
    }
}}