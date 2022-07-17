#pragma once

//comes from Brendan Galea

#include "Croogine_Device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace Croogine {

    class CroogineDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(CroogineDevice &croogineDevice) : croogineDevice{ croogineDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<CroogineDescriptorSetLayout> build() const;

        private:
            CroogineDevice& croogineDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        CroogineDescriptorSetLayout(
            CroogineDevice& croogineDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~CroogineDescriptorSetLayout();
        CroogineDescriptorSetLayout(const CroogineDescriptorSetLayout&) = delete;
        CroogineDescriptorSetLayout& operator=(const CroogineDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        CroogineDevice& croogineDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class CroogineDescriptorWriter;
    };

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