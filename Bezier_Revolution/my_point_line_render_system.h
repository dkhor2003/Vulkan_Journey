#ifndef __MY_POINT_LINE_RENDER_SYSTEM_H__
#define __MY_POINT_LINE_RENDER_SYSTEM_H__

#include "my_device.h"
#include "my_game_object.h"
#include "my_pipeline.h"
#include "my_camera.h"
#include "my_frame_info.h"

// std
#include <memory>
#include <vector>


class MyPointLineRenderSystem
{
public:
	MyPointLineRenderSystem(MyDevice& device, VkRenderPass renderPass, VkPrimitiveTopology topology);
	~MyPointLineRenderSystem();

	MyPointLineRenderSystem(const MyPointLineRenderSystem&) = delete;
	MyPointLineRenderSystem& operator=(const MyPointLineRenderSystem&) = delete;

	void renderControlPoints(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);
	void renderCenerLine(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);
	void renderBezierCurve(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);
	void renderNormals(MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);

private:
	void _createPipelineLayout();
	void _createPipeline(VkRenderPass renderPass, VkPrimitiveTopology topology);
	void _renderPointsLines(std::string name, MyFrameInfo& frameInfo, std::vector<MyGameObject>& gameObjects);

	MyDevice&                   m_myDevice;

	std::unique_ptr<MyPipeline> m_pMyPipeline;
	VkPipelineLayout            m_vkPipelineLayout;
};

#endif

