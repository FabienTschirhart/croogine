#pragma once

//comes from Brendan Galea
#include <memory>


#include "Croogine_DescriptorSetLayout.h"
#include "Croogine_DescriptorPool.h"

namespace Croogine {
    class CroogineDescriptorWriter {
    public:
        CroogineDescriptorWriter(CroogineDescriptorSetLayout& setLayout, CroogineDescriptorPool& pool);

        CroogineDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        CroogineDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        CroogineDescriptorSetLayout& setLayout;
        CroogineDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}