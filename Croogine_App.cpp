#include "Croogine_App.h"
#include "Croogine_KB_Controller.h"
#include "Croogine_Camera.h"
#include "Croogine_RenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace Croogine {

    CroogineApp::CroogineApp() {
        loadEntities();
    }

    CroogineApp::~CroogineApp() {}

	void CroogineApp::run() {

        CroogineRenderSystem renderSystem{ croogineDevice, croogineRenderer.getSwapChainRenderPass() };
        CroogineCamera camera{};

        auto cameraEntity = CroogineEntity::createEntity();
        KeyboardController cameraEntity_Controller{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameDuration = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;    

            frameDuration = glm::min(frameDuration, MAX_FRAME_DURATION);

            cameraEntity_Controller.move(croogineWindow.getGLFWwindow(), frameDuration, cameraEntity);
            camera.setViewYXZ(cameraEntity.transform.translation, cameraEntity.transform.rotation);
                
            float aspect = croogineRenderer.getAspectRatio();

            if(ORTHOGRAPHIC_PROJECTION)
                camera.setOrthographicProjection(-aspect, aspect, ORTHOGRAPHIC_TOP, ORTHOGRAPHIC_BOTTOM, ORTHOGRAPHIC_NEAR_PLANE, ORTHOGRAPHIC_FAR_PLANE);
            else
                camera.setPerspectiveProjection(FOV, aspect, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
            
            entities[0].transform.rotation += glm::vec3{ 0.f, 0.00005f , 0.f };

            if (auto commandBuffer = croogineRenderer.beginFrame()) {
                croogineRenderer.beginSCRenderPass(commandBuffer);
                renderSystem.renderEntities(commandBuffer, entities, camera);
                croogineRenderer.endSCRenderPass(commandBuffer);
                croogineRenderer.endFrame();
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
}