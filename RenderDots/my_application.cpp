#include "my_application.h"
#include <stdexcept>
#include <array>
#include <iostream>


struct ColorPushConstant
{
    glm::vec3 color;
};

MyApplication::MyApplication()
{
    _createModel();
    _createPipelineLayout();
    _createPipeline();
    _createCommandBuffers();
    m_colorIndex = 0; 
}

MyApplication::~MyApplication()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MyApplication::run() 
{
    m_myWindow.bindMyApplication(this);

    while (!m_myWindow.shouldClose()) 
    {
        m_myWindow.pollEvents();
        _updateModel();
        _drawFrame();
    }

    // GPU will block until all CPU is complete
    vkDeviceWaitIdle(m_myDevice.device());
}

void MyApplication::_createModel()
{
    m_pMyModel = std::make_unique<MyModel>(m_myDevice);
}

void MyApplication::_createPipelineLayout()
{
    VkPushConstantRange pushContantRange{};

    // We would like to push the constant data to vertex shader only
    pushContantRange.stageFlags =  VK_SHADER_STAGE_FRAGMENT_BIT;
    pushContantRange.offset = 0;
    pushContantRange.size = sizeof(ColorPushConstant);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1; 
    pipelineLayoutInfo.pPushConstantRanges = &pushContantRange; 

    if (vkCreatePipelineLayout(m_myDevice.device(), &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline layout!");
    }
}

void MyApplication::_createPipeline()
{
    // Note: the swap chain buffer size may not be the same as the Window size
    // Pipeline config can be considered as a blue print to create graphics pipeline
    PipelineConfigInfo pipelineConfig{};
        
    MyPipeline::defaultPipelineConfigInfo(pipelineConfig, m_mySwapChain.width(), m_mySwapChain.height());

    // For now render pass contains the structure and the format
    // of the frame buffer object and its attachments
    // for example in the defect FBO, attachment 0 is the color buffer
    // and attachment 1 is the depth buffer
    pipelineConfig.renderPass = m_mySwapChain.renderPass();
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline = std::make_unique< MyPipeline >(
        m_myDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig
        );
}

void MyApplication::_createCommandBuffers()
{
    // Command buffers contain one or multiple commends that are to be executed on the GPU
    // 
    // 1:1 mapping between the command buffer and the frame buffer for now
    // Could be either 2 or 3 depending on the graphics hardware
    m_vVkCommandBuffers.resize(m_mySwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_myDevice.commandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_vVkCommandBuffers.size());

    if (vkAllocateCommandBuffers(m_myDevice.device(), &allocInfo, m_vVkCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void MyApplication::_recordCommandBuffer(int imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_vVkCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffers!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_mySwapChain.renderPass();
    renderPassInfo.framebuffer = m_mySwapChain.frameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_mySwapChain.swapChainExtent();

    // becuse index 0 is color buffer and 1 is depth buffer
    // so we only need to clear the proper index only
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.1f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    // VK_SUBPASS_CONTENTS_INLINE means only use Primary command buffer. No secondary command buffer
    vkCmdBeginRenderPass(m_vVkCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    m_pMyPipeline->bind(m_vVkCommandBuffers[imageIndex]);

    ColorPushConstant colordata{}; 

    if (m_colorIndex == m_Color.size())
    {
        colordata.color.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        colordata.color.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        colordata.color.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
    else
    {
        colordata.color = m_Color[m_colorIndex]; 
    }

    vkCmdPushConstants(m_vVkCommandBuffers[imageIndex],
                       m_vkPipelineLayout, 
					   VK_SHADER_STAGE_FRAGMENT_BIT,  // push to fragment shader only
                       0, sizeof(ColorPushConstant), &colordata);

    m_pMyModel->bind(m_vVkCommandBuffers[imageIndex]);

    // Don't draw if clearing window screen
    if (!m_reset)
    {
        m_pMyModel->draw(m_vVkCommandBuffers[imageIndex]);
    }

    vkCmdEndRenderPass(m_vVkCommandBuffers[imageIndex]);
    if (vkEndCommandBuffer(m_vVkCommandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to recording command buffers!");
    }
}

void MyApplication::_drawFrame()
{
    uint32_t imageIndex = 0;
    auto result = m_mySwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    _recordCommandBuffer(imageIndex);

    // Submit the command buffer to the graphics queue
    // the GPU and CPU synchronization will be handled properly
    result = m_mySwapChain.submitCommandBuffers(&m_vVkCommandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit comamnd buffers!");
    }
}

void MyApplication::_updateModel()
{
    // Only send vector of vertices to GPU if there is a new point
    if (m_changeInVertices)
    {
        m_pMyModel->UpdateVertices(m_vVertices); 
        m_changeInVertices = false; 
    }
}

void MyApplication::AddPoint(float x, float y)
{
    if (m_vVertices.size() >= m_pMyModel->getNumMaxVertices())
    {
        std::cout << "Maximum number of points has reached ! To continue rendering new points, hit spacebar to clear all the points." << std::endl; 
        std::cout << "-------------------------------------------------" << std::endl; 
        return; 
    }

    float vk_x = (x / (WIDTH / 2)) - 1.0f; 
    float vk_y = (y / (HEIGHT / 2)) - 1.0f; 
    std::cout << "Point " << m_vVertices.size() + 1 << " created with following coordinates: \n"
              << "\tOriginal x: " << x << "    Vulkan x: " << vk_x << "\n"
              << "\tOriginal y: " << y << "    Vulkan y = " << vk_y << "\n"
              << "-------------------------------------------------" << std::endl;
    
    MyModel::Vertex newPoint; 

    newPoint.position.x = vk_x; 
    newPoint.position.y = vk_y; 

    m_vVertices.push_back(newPoint); 
    m_changeInVertices = true; 
    m_reset = false; 
}

void MyApplication::Reset()
{
    std::cout << "My Application: Reset view! Cleared a total of " << m_vVertices.size() << " points." << std::endl;
    std::cout << "-------------------------------------------------" << std::endl; 

    m_vVertices.clear(); 
    m_changeInVertices = true;
    m_reset = true; 
}

void MyApplication::ColorSwitch()
{
    std::cout << "Switching Color!";

    if (m_colorIndex + 1 == m_Color.size())
    {
        std::cout << " Entering dazzling mode..."; 
    }
    std::cout << "\n-------------------------------------------------" << std::endl; 
    
    if (m_colorIndex == m_Color.size())
    {
        m_colorIndex = 0; 
    }
    else 
    {
        m_colorIndex += 1; 
    }
}

