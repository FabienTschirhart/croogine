#include "Croogine_App.h"
#include "Croogine_KB_Controller.h"
#include "Croogine_Camera.h"
#include "Croogine_Buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace Croogine {

    struct GlobalUniformBufferObject {
        glm::mat4 projectionView{ 1.f };
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };

    CroogineApp::CroogineApp() {
        loadEntities();
    }

    CroogineApp::~CroogineApp() {}

	void CroogineApp::run() {

        std::vector<std::unique_ptr<CroogineBuffer>> uboBuffers(CroogineSwapChain::MAX_FRAMES_IN_FLIGHT);
        CroogineRenderSystem renderSystem{ croogineDevice, croogineRenderer.getSwapChainRenderPass() };
        CroogineCamera camera{};
        auto cameraEntity = CroogineEntity::createEntity();
        KeyboardController cameraEntity_Controller{};
        auto currentTime = std::chrono::high_resolution_clock::now();

        initUBO(uboBuffers);

        while (!croogineWindow.shouldClose()) { //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameDuration = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;    
            frameDuration = glm::min(frameDuration, MAX_FRAME_DURATION);         
            
            if (auto commandBuffer = croogineRenderer.beginFrame()) {

                Frame frame{
                    croogineRenderer.getFrameIndex(),
                    frameDuration,
                    commandBuffer,
                    camera
                };

                updateCamera(frame, cameraEntity_Controller, cameraEntity);
                update(frame, uboBuffers); 
                render(frame, renderSystem);
            }
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());
	}

    void CroogineApp::loadEntities() {
        
        //std::shared_ptr<CroogineModel> croogineModel = createCubePrimitive(croogineDevice, { .0f, .0f, .0f });
        std::shared_ptr<CroogineModel> croogineModel = CroogineModel::createModel(croogineDevice, MODEL_CORNELL_BOX_PATH, false);

        auto element = CroogineEntity::createEntity();
        element.model = croogineModel;
        element.transform.translation = { 0.f, 1.75f, 7.f };
        element.transform.scale = { 1.f, 1.f, 1.f };
        element.transform.rotation = { 1.5708f, 0.f, 0.f };

        entities.push_back(std::move(element));
    }

    void CroogineApp::initUBO(std::vector<std::unique_ptr<CroogineBuffer>>& UniformBufferObject)
    {
        for (int i = 0; i < UniformBufferObject.size(); i++)
        {
            UniformBufferObject[i] = std::make_unique<CroogineBuffer>(
                croogineDevice,
                sizeof(GlobalUniformBufferObject),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            UniformBufferObject[i]->map();
        }
    }

    void CroogineApp::updateCamera(Frame& frame, KeyboardController& cameraController, CroogineEntity& camera)
    {
        cameraController.move(croogineWindow.getGLFWwindow(), frame.frameDuration, camera);
        frame.camera.setViewYXZ(camera.transform.translation, camera.transform.rotation);

        float aspect = croogineRenderer.getAspectRatio();

        if (ORTHOGRAPHIC_PROJECTION)
            frame.camera.setOrthographicProjection(-aspect, aspect, ORTHOGRAPHIC_TOP, ORTHOGRAPHIC_BOTTOM, ORTHOGRAPHIC_NEAR_PLANE, ORTHOGRAPHIC_FAR_PLANE);
        else
            frame.camera.setPerspectiveProjection(FOV, aspect, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
    }

    void CroogineApp::update(Frame& frame, std::vector<std::unique_ptr<CroogineBuffer>>& UniformBufferObject)
    {
        GlobalUniformBufferObject ubo{};

        ubo.projectionView = frame.camera.getProjection() * frame.camera.getView();
        UniformBufferObject[frame.frameIndex]->writeToBuffer(&ubo);
        UniformBufferObject[frame.frameIndex]->flush();

        //entities[0].transform.rotation += glm::vec3{ 0.f, 0.00005f , 0.f };

    }

    void CroogineApp::render(Frame& frame, CroogineRenderSystem& renderSystem)
    {
        croogineRenderer.beginSCRenderPass(frame.commandBuffer);
        renderSystem.renderEntities(frame, entities);
        croogineRenderer.endSCRenderPass(frame.commandBuffer);
        croogineRenderer.endFrame();
    }
}