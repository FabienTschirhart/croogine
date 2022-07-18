#include "Croogine_DescriptorSetLayout.h"

// std

namespace Croogine {

    // *************** Descriptor Set Layout Builder *********************

    CroogineDescriptorSetLayout::Builder& CroogineDescriptorSetLayout::Builder::addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count) {
        assert(bindings.count(binding) == 0 && "Binding already in use");
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = count;
        layoutBinding.stageFlags = stageFlags;
        bindings[binding] = layoutBinding;
        return *this;
    }

    std::unique_ptr<CroogineDescriptorSetLayout> CroogineDescriptorSetLayout::Builder::build() const {
        return std::make_unique<CroogineDescriptorSetLayout>(croogineDevice, bindings);
    }

    // *************** Descriptor Set Layout *********************

    CroogineDescriptorSetLayout::CroogineDescriptorSetLayout(
        CroogineDevice& croogineDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
        : croogineDevice{ croogineDevice }, bindings{ bindings } {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
        for (auto kv : bindings) {
            setLayoutBindings.push_back(kv.second);
        }

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
        descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

        if (vkCreateDescriptorSetLayout(
            croogineDevice.getDevice(),
            &descriptorSetLayoutInfo,
            nullptr,
            &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    CroogineDescriptorSetLayout::~CroogineDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(croogineDevice.getDevice(), descriptorSetLayout, nullptr);
    }

}