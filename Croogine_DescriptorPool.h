#pragma once

//comes from Brendan Galea

#include "Croogine_DescriptorSetLayout.h"
#include "Croogine_Device.h"
#include <stdexcept>


namespace Croogine {
    class CroogineDescriptorPool {
    public:
        class Builder {
        public:
            Builder(CroogineDevice& croogineDevice) : croogineDevice{ croogineDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<CroogineDescriptorPool> build() const;

        private:
            CroogineDevice& croogineDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        CroogineDescriptorPool(
            CroogineDevice& croogineDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~CroogineDescriptorPool();
        CroogineDescriptorPool(const CroogineDescriptorPool&) = delete;
        CroogineDescriptorPool& operator=(const CroogineDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        CroogineDevice& croogineDevice;
        VkDescriptorPool descriptorPool;

        friend class CroogineDescriptorWriter;
    };
}