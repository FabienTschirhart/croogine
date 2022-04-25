#pragma once

#include "Croogine_Device.h"

namespace Croogine {

	class CroogineModel {

	public:
		CroogineModel();
		~CroogineModel();

		CroogineModel(const CroogineModel&) = delete;
		CroogineModel& operator=(const CroogineModel&) = delete;



	private :
		CroogineDevice croogineDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}