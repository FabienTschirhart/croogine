#pragma once

#ifndef CONSTANTS
#define CONSTANTS


#include <string>


const int VERSION_VARIANT = 0;
const int VERSION_MAJOR = 2;
const int VERSION_MINOR = 0;
const int VERSION_PATCH = 0;

const std::string ENGINE_NAME = "Croogine 3D";
const std::string ENGINE_VERSION = "V0.1.0";

const std::string vertexshader_file = "shaders/vert.spv";
const std::string fragmentshader_file = "shaders/frag.spv";
const std::string texture_file = "textures/Woman_Punk.jpg";

const std::string MODEL_PATH = "models/cornell_box_Custom.obj";
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

#endif