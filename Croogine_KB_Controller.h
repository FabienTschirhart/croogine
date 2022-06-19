#pragma once

#include "Croogine_Entity.h"
#include "Croogine_window.h"

namespace Croogine {

    class KeyboardController {

    public:

        struct KeyboardMapping {

            int moveForward = GLFW_KEY_KP_8;
            int moveBackward = GLFW_KEY_KP_5;
            int moveLeft = GLFW_KEY_KP_4;
            int moveRight = GLFW_KEY_KP_6;

            int moveUp = GLFW_KEY_KP_9;
            int moveDown = GLFW_KEY_KP_7;

            int lookLeft = GLFW_KEY_KP_1;
            int lookRight = GLFW_KEY_KP_3;
            int lookUp = GLFW_KEY_PAGE_UP;
            int lookDown = GLFW_KEY_PAGE_DOWN;

            int reset = GLFW_KEY_DELETE;
        };

        KeyboardMapping keys{};
        float moveSpeed = MOVEMENT_SPEED;
        float rotationSpeed = ROTATION_SPEED;

        void move(GLFWwindow* window, float dt, CroogineEntity& entity);

    };
}