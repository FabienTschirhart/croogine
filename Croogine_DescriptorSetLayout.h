#pragma once

//comes from Brendan Galea

#include "Croogine_Device.h"

// std
#include <memory>
#include <unordered_map>
#include <stdexcept>

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
}