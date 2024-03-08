#ifndef __MY_APPLICATION_H__
#define __MY_APPLICATION_H__

#include "my_window.h"
#include "my_pipeline.h"
#include "my_device.h"
#include "my_swap_chain.h"
#include "my_model.h"

#include <string>
#include <memory>
#include <vector>

class MyApplication 
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	MyApplication();
	~MyApplication();

	MyApplication(const MyApplication&) = delete;
	MyApplication& operator=(const MyApplication&) = delete;

	void AddPoint(float x, float y);
	void Reset();
	void ColorSwitch();

	void run();

private:
    void _createModel();
	void _updateModel();
	void _createPipelineLayout();
    void _createPipeline();
	void _createCommandBuffers();
	void _drawFrame();
    void _recordCommandBuffer(int imageIndex);

	MyWindow                     m_myWindow{ WIDTH, HEIGHT, "Assignment 1" };
	MyDevice                     m_myDevice{ m_myWindow };
	MySwapChain                  m_mySwapChain{ m_myDevice, m_myWindow.extent() };

	std::unique_ptr<MyPipeline>  m_pMyPipeline;
	
	VkPipelineLayout             m_vkPipelineLayout;
	std::vector<VkCommandBuffer> m_vVkCommandBuffers;
	std::unique_ptr<MyModel>     m_pMyModel;
	std::vector<MyModel::Vertex> m_vVertices; 
	std::vector<glm::vec3>       m_Color {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	}; 

	uint32_t                     m_colorIndex; 

	bool                         m_changeInVertices = false; 
	bool                         m_reset = false; 
};

#endif

