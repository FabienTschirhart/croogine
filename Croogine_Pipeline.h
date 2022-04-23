#pragma once

#include "Croogine_Device.h"

#include <string>
#include <vector>

namespace Croogine {

	struct PipelineConfigInfo {};

	class CrooginePipeline {
		public:
			CrooginePipeline(CroogineDevice &device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo &configInfo);
			~CrooginePipeline();

			CrooginePipeline(const CrooginePipeline&) = delete;
			void operator=(const CrooginePipeline&) = delete;

			static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

		private:		

			void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

			void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

			CroogineDevice& CroogineDevice;
			VkPipeline graphicsPipeline;
			VkShaderModule vertexShaderModule;
			VkShaderModule fragmentShaderModule;
	};
}
