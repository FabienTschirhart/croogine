#pragma once

#include "Croogine_Model.h"


#include <memory>

namespace Croogine {

	struct Transform2d {
		glm::vec2 translation{};


		glm::mat2 mat2() { return glm::mat2(1.f); }
	};

	class CroogineEntity {
	
	public:
		using id_t = unsigned int;

		static CroogineEntity createEntity() {
			static id_t currentId = 0;
			return CroogineEntity{ currentId++ };
		}

		CroogineEntity(const CroogineEntity&) = delete;
		CroogineEntity& operator=(const CroogineEntity&) = delete;
		CroogineEntity(CroogineEntity&&) = default;
		CroogineEntity& operator=(CroogineEntity&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<CroogineModel> model{};
		glm::vec3 color{};
		Transform2d transform2D{};

	private:
		CroogineEntity(id_t entityId): id(entityId) {}
		id_t id;
	
	
	
	};

}
