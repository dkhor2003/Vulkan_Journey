#include "my_model.h"
#include <cassert>

MyModel::MyModel(MyDevice& device) :
	m_myDevice{ device },
	m_iVertexCount{ 0 }
{
	_createVertexBuffer();
}

MyModel::~MyModel()
{
    vkDestroyBuffer(m_myDevice.device(), m_vkVertexBuffer, nullptr);
    vkFreeMemory(m_myDevice.device(), m_vkVertexBufferMemory, nullptr);
}

void MyModel::_createVertexBuffer()
{  
    // // number of bytes need to store the vertex buffer
    // // Note: we assume Color and Position are interleaved here
    // // inside the vertex buffer
    m_myBufferSize = sizeof(glm::vec2) * 100;
    
    // // Create buffer handle and allocate buffer memory on GPU side
    // // Note: Host - CPU
    // //       Device - GPU
    // // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT - make sure the GPU memory is visible to the host
    // // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT - make sure the host and device memory is consistent
    m_myDevice.createBuffer(
    	m_myBufferSize,
    	VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    	m_vkVertexBuffer,
    	m_vkVertexBufferMemory);
}

void MyModel::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = { m_vkVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void MyModel::draw(VkCommandBuffer commandBuffer)
{
    vkCmdDraw(commandBuffer, m_iVertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription> MyModel::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> MyModel::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = 0;
	return attributeDescriptions;
}

uint32_t MyModel::getNumMaxVertices()
{
    return 100; // Maximum of 100 points to be rendered
}

void MyModel::UpdateVertices(const std::vector<Vertex> &vertices)
{
    m_iVertexCount = static_cast<uint32_t>(vertices.size());
    void* data;
    vkMapMemory(m_myDevice.device(), m_vkVertexBufferMemory, 0, m_myBufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(m_myBufferSize));
    vkUnmapMemory(m_myDevice.device(), m_vkVertexBufferMemory);
}



