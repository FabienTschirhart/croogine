#include "Croogine_Pipeline.h"
#include "Utils.h"

// std
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace Croogine {

    CrooginePipeline::CrooginePipeline(
        Croogine::CroogineDevice& device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo)
        : CroogineDevice{ device }
    {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }

    CrooginePipeline::~CrooginePipeline()
    {
        vkDestroyShaderModule(CroogineDevice.getDevice(), fragmentShaderModule, nullptr);
        vkDestroyShaderModule(CroogineDevice.getDevice(), vertexShaderModule, nullptr);
        vkDestroyPipeline(CroogineDevice.getDevice(), graphicsPipeline, nullptr);
    }

    void CrooginePipeline::createGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo) {

        auto vertCode = CroogineUtils::readFile(vertFilepath);
        auto fragCode = CroogineUtils::readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }

    void CrooginePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(CroogineDevice.getDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

    }

    PipelineConfigInfo CrooginePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};

        return configInfo;
    }


}
