#include "Croogine_Pipeline.h"
#include "Utils.h"

// std
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace Croogine {

    CrooginePipeline::CrooginePipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
        createGraphicsPipeline(vertFilepath, fragFilepath);
    }

    void CrooginePipeline::createGraphicsPipeline(
        const std::string& vertFilepath, 
        const std::string& fragFilepath) {

        auto vertCode = CroogineUtils::readFile(vertFilepath);
        auto fragCode = CroogineUtils::readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }

}