#include "my_simple_render_system.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

struct MySimplePushConstantData
{
    glm::mat2 transform{ 1.f };
    glm::vec2 offset;
};

MySimpleRenderSystem::MySimpleRenderSystem(MyDevice& device, VkRenderPass renderPass)
    : m_myDevice{ device } 
{
    _createPipelineLayout();
    _createPipeline(renderPass);
}

MySimpleRenderSystem::~MySimpleRenderSystem()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MySimpleRenderSystem::_createPipelineLayout() 
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(MySimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(m_myDevice.device(), &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void MySimpleRenderSystem::_createPipeline(VkRenderPass renderPass)
{
    assert(m_vkPipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    MyPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline = std::make_unique<MyPipeline>(
        m_myDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

void MySimpleRenderSystem::renderGameObjects(
    VkCommandBuffer commandBuffer, std::vector<MyGameObject>& gameObjects)
{
    m_pMyPipeline->bind(commandBuffer);

    for (auto& obj : gameObjects)
    {    
        MySimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.transform = obj.transform2d.mat2();

        vkCmdPushConstants(
            commandBuffer,
            m_vkPipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(MySimplePushConstantData),
            &push);

        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}

