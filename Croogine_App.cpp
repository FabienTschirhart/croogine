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
#include <iostream>

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
            

            if (auto commandBuffer = croogineRenderer.beginFrame()) {
                croogineRenderer.beginSCRenderPass(commandBuffer);
                renderSystem.renderEntities(commandBuffer, entities, camera);
                croogineRenderer.endSCRenderPass(commandBuffer);
                croogineRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());
	}

    std::unique_ptr<CroogineModel> createCubePrimitive(CroogineDevice& device, glm::vec3 offset) {

        CroogineModel::Modeler model{};
        model.vertices =

        model.vertices = {

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : model.vertices) {
            v.position += offset;
        }
        return std::make_unique<CroogineModel>(device, model);
    }

    void CroogineApp::loadEntities() {
        std::shared_ptr<CroogineModel> croogineModel = createCubePrimitive(croogineDevice, { .0f, .0f, .0f });

        auto cube = CroogineEntity::createEntity();
        cube.model = croogineModel;
        cube.transform.translation = { .0f, .0f, 2.5f };
        cube.transform.scale = { .5f, .5f, .5f };

        entities.push_back(std::move(cube));
    }
}