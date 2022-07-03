#pragma once

#ifndef CONSTANTS
#define CONSTANTS


#include <string>
#include <glm/glm.hpp>

const int VERSION_VARIANT = 0;
const int VERSION_MAJOR = 2;
const int VERSION_MINOR = 3;
const int VERSION_PATCH = 0;

const std::string ENGINE_NAME = "Croogine 3D";
const std::string ENGINE_VERSION = " V0.2.3.0";

const std::string VERTEX_SHADER_FILE = "shaders/vert.spv";
const std::string FRAGMENT_SHADER_FILE = "shaders/frag.spv";
const std::string TEXTURE_FILE = "textures/Woman_Punk.jpg";

const std::string MODEL_CORNELL_BOX_PATH = "models/cornell_box_Custom.obj";
const std::string MODEL_COLORED_CUBE_PATH = "models/colored_cube.obj";
const std::string MODEL_CUBE_PATH = "models/cube.obj";
const std::string MODEL_FLAT_VASE = "models/flat_vase.obj";
const std::string MODEL_SMOOTH_VASE = "models/smooth_vase.obj";
const std::string TEXTURE_PATH = "textures/Cornell_Box_Lightmap_2048.png";
//"textures/Cornell_Box_Flat.png"
//"textures/Cornell_Box_BaseColor.png"
//"textures/Cornell_Box_Lightmap_512_DirectLighting.png"
//"textures/Cornell_Box_Lightmap_512.png"
//"textures/Cornell_Box_Lightmap_2048.png"

//const uint32_t WIDTH = 1920;
//const uint32_t HEIGHT = 1080;

static constexpr int WIDTH = 1024;
static constexpr int HEIGHT = 768;

const int MAX_FRAMES_IN_FLIGHT = 2;

//const VkClearColorValue backgroundColor = { 0.f, 0.f, 0.f, 1.f };

/* CAMERA */

const bool ORTHOGRAPHIC_PROJECTION = false;

const float FOV = glm::radians(50.f); //field of view

const float ORTHOGRAPHIC_TOP = -1;
const float ORTHOGRAPHIC_BOTTOM = 1;

const float ORTHOGRAPHIC_NEAR_PLANE = -1;
const float ORTHOGRAPHIC_FAR_PLANE = 1;
const float PERSPECTIVE_NEAR_PLANE = 0.1f;
const float PERSPECTIVE_FAR_PLANE = 10.f;

/* MOVEMENT */

const float MOVEMENT_SPEED = 3.0;
const float ROTATION_SPEED = 1.5f;

/* FRAME */

const float MAX_FRAME_DURATION = 0.01f;


#endif