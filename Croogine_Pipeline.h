#pragma once

#include <string>
#include <vector>

namespace Croogine {
	class CrooginePipeline {
	public:
		CrooginePipeline(const std::string& vertFilepath, const std::string& fragFilepath);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
	};
}
