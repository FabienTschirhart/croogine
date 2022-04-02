#pragma once
#ifndef CROOGINE_CONSTANTS
#define CROOGINE_CONSTANTS

#include "Croogine_Enums.h"
#include "Croogine_VKVertex.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int VERSION_VARIANT = 0;
const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;
const int VERSION_PATCH = 0;

const std::string ENGINE_NAME = "Croogine 3D";
const std::string ENGINE_VERSION = "V0.0.1";

const std::string vertexshader_file = "shaders/vert.spv";
const std::string fragmentshader_file = "shaders/frag.spv";
const std::string texture_file = "textures / Woman_Punk.jpg";

const uint32_t WIDTH = 1024;
const uint32_t HEIGHT = 768;

const GL_API SELECTED_GL_API = Vulkan;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};
#endif