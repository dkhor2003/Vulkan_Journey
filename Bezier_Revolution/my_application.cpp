#include "my_application.h"

// Render system
#include "my_simple_render_system.h"
#include "my_point_line_render_system.h"
#include "my_keyboard_controller.h"

// use radian rather degree for angle
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Std
#include <stdexcept>
#include <array>
#include <chrono>
#include <iostream>
#include <limits>
#include <math.h>

MyApplication::MyApplication() :
    m_bPerspectiveProjection(true)
{
    _loadGameObjects();
}

MyApplication::~MyApplication()
{

}

void MyApplication::run() 
{
    // Declare the render systems
    MySimpleRenderSystem    simpleRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass() };                                  // Draw indexed triangles
    MyPointLineRenderSystem pointRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), VK_PRIMITIVE_TOPOLOGY_POINT_LIST }; // Draw points
    MyPointLineRenderSystem lineRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), VK_PRIMITIVE_TOPOLOGY_LINE_STRIP };  // Draw line strip
    MyPointLineRenderSystem normalRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass(), VK_PRIMITIVE_TOPOLOGY_LINE_LIST }; // Draw lines

    m_myWindow.bindMyApplication(this);

    // Switch to edit mode initially
    switchEditMode();

    // Empty object
    auto viewerObject = MyGameObject::createGameObject("camera");
    MyKeyboardController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!m_myWindow.shouldClose()) 
    {
        // Note: depending on the platforms (PC, Linux or Mac), this function
        // will cause the event proecssing to block during a Window move, resize or
        // menu operation. Users can use the "window refresh callback" to redraw the
        // contents of the window when necessary during such operation.
        m_myWindow.pollEvents();

        // Need to get the call after glfwPollEvants because the call above may take time
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        // Don't use keyboard to move camera for this assginment
        //cameraController.moveInPlaneXZ(m_myWindow.glfwWindow(), frameTime, viewerObject);
        //m_myCamera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float apsectRatio = m_myRenderer.aspectRatio();

        // Put it here because the viewport may change
		//std::cout << "Aspect ratio: " << apsectRatio << std::endl;

        if (m_bPerspectiveProjection)
            // near and far will automatically apply negative values
            m_myCamera.setPerspectiveProjection(glm::radians(50.f), apsectRatio, 0.1f, 100.f);
        else
            // because Y is down by default for Vulkan, when we set top to be minus value, we can flip the coordinate
            // such that Y is up. Because we move the part 2.5 units, the near and far value needs to cover the model
            // Also, near and far will automatically apply negative values
            m_myCamera.setOrthographicProjection(-apsectRatio, apsectRatio, -1.0f, 1.0f, -100.0f, 100.0f);
 
        // Please note that commandBuffer could be null pointer
        // if the swapChain needs to be recreated
        if (auto commandBuffer = m_myRenderer.beginFrame())
		{
            // In case we have multiple render passes for the current frame
            // begin offsreen shadow pass
            // render shadow casting objects
            // end offscreen shadow pass

            int frameIndex = m_myRenderer.frameIndex();
            MyFrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, m_myCamera, glm::vec3(1.0f, 1.0f, 1.0f) };
            m_myRenderer.beginSwapChainRenderPass(commandBuffer);
            
            // The order the render systems determines which one is rendered on top
            frameInfo.color = glm::vec3(1.0f, 0.0f, 0.0f);
            pointRenderSystem.renderControlPoints(frameInfo, m_vMyGameObjects);

            frameInfo.color = glm::vec3(1.0f, 1.0f, 0.0f);
            lineRenderSystem.renderControlPoints(frameInfo, m_vMyGameObjects);

            frameInfo.color = glm::vec3(0.0f, 0.0f, 1.0f);
            lineRenderSystem.renderCenerLine(frameInfo, m_vMyGameObjects);

            frameInfo.color = glm::vec3(1.0f, 1.0f, 1.0f);
            lineRenderSystem.renderBezierCurve(frameInfo, m_vMyGameObjects);

            if (m_bShowNormals) // render normal vectors only if users decided to show the normals
			{
                frameInfo.color = glm::vec3(0.0f, 1.0f, 0.0f);
                normalRenderSystem.renderNormals(frameInfo, m_vMyGameObjects);
            }

            if (m_bShowSurface) // render surface only if users decided to show the surface
            {
                frameInfo.color = glm::vec3(1.0f, 1.0f, 1.0f); // Not use
                simpleRenderSystem.renderGameObjects(frameInfo, m_vMyGameObjects);
            }

            m_myRenderer.endSwapChainRenderPass(commandBuffer);

            m_myRenderer.endFrame();
        }
    }

    // GPU will block until all CPU is complete
    vkDeviceWaitIdle(m_myDevice.device());
}

void MyApplication::switchProjectionMatrix()
{
    // Switch between perspective and orthographic projection matrix
    m_bPerspectiveProjection = !m_bPerspectiveProjection;
}

void MyApplication::_loadGameObjects()
{
    // Initialize the memory of Game Objects
    m_pMyBezier = std::make_shared<MyBezier>();

    // Add dynamic control points (no more than 100 points)
    std::shared_ptr<MyModel> mypontLine = std::make_shared<MyModel>(m_myDevice, 100);
    auto mygameobj1 = MyGameObject::createGameObject("control_points");
    mygameobj1.model = mypontLine;
    m_vMyGameObjects.push_back(std::move(mygameobj1));

    // Add center line (2 points)
    std::shared_ptr<MyModel> mycenterLine = std::make_shared<MyModel>(m_myDevice, 2);
    auto mygameobj2 = MyGameObject::createGameObject("centerLine");
    mygameobj2.model = mycenterLine;

    MyModel::PointLine vertex1, vertex2;
    vertex1.position.x = -1.0f;
    vertex1.position.y = 0.0f;
    vertex1.position.z = 0.0f;
    vertex1.color.x = 0.0f;
    vertex1.color.y = 0.0f;
    vertex1.color.z = 1.0f;
    vertex2.position.x = 1.0f;
    vertex2.position.y = 0.0f;
    vertex2.position.z = 0.0f;
    vertex2.color.x = 0.0f;
    vertex2.color.y = 0.0f;
    vertex2.color.z = 1.0f;

    std::vector<MyModel::PointLine> centerLine;

    centerLine.push_back(vertex1);
    centerLine.push_back(vertex2);

    mycenterLine->updatePointLines(centerLine);

    m_vMyGameObjects.push_back(std::move(mygameobj2));

	// Note: if you increase the resolutions of the curve, you need to increase the value 1000 here
    std::shared_ptr<MyModel> mybezierCurve = std::make_shared<MyModel>(m_myDevice, 1000);
    auto mygameobj3 = MyGameObject::createGameObject("bezier_curve");
    mygameobj3.model = mybezierCurve;
    m_vMyGameObjects.push_back(std::move(mygameobj3));

    // Add revolution surface game object, but no model yet
    auto mygameobj4 = MyGameObject::createGameObject("bezier_surface"); // it will crash if don't check nullptr on model in the render system
    m_vMyGameObjects.push_back(std::move(mygameobj4));

    // Add normal vectors game object, but no model yet
    auto mygameobj5 = MyGameObject::createGameObject("surface_normals");
    m_vMyGameObjects.push_back(std::move(mygameobj5));
}

// Function used to check whether the given coordinate overlaps with an existing control point, if yes return its index in m_vControlPointVertices
int  MyApplication::_queryControlPoints(float posx, float posy)
{
    for (int i = 0; i < m_vControlPointVertices.size(); i++)
    {
        // Tolerate offsets within 0.01 range of actual coordinate
        if (fabs(m_vControlPointVertices[i].position.x - posx) <= 0.01 && fabs(m_vControlPointVertices[i].position.y - posy) <= 0.01)
        {
            return i;
        }
    }

    return -1; 
}

void MyApplication::mouseButtonEvent(bool bMouseDown, float posx, float posy)
{
    if (m_bCreateModel && bMouseDown)
    {
        // Don't add the control point below the center line
        if (posy >= 0.5f)
        {
            return;
        }

		// Convert position from (0, 1) to (-1, 1) and flip Y (before = down is positive, now = up is positive)

        //            (0, 0)               (1, 0)               (-1, 1)                (1, 1)
        //             ------------------------                   ------------------------
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             |                      |      ----->       |                      |
        //             |     WINDOW           |                   |      WINDOW          |
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             |                      |                   |                      |
        //             ------------------------                   ------------------------
        //            (0, 1)               (1, 1)               (-1, -1)               (1, -1)       

		float x = 2.0f * posx - 1.0f;
        float y = -2.0f * posy + 1.0f;

        std::cout << "Mouse button pressed" << std::endl;
        m_bMoving = true;

        int index = _queryControlPoints(x, y);

        // If currently clicking on an existing control point, set it such that its position can be controlled by user depending on mouse position
        if (index != -1)
        {
            std::cout << "Selected control point: " << "(" << m_vControlPointVertices[index].position.x << ", " << m_vControlPointVertices[index].position.y << ")" << std::endl; 
            index_of_selected_point = index; 
        }

        // Otherwise, create a new control point
        else 
        {
            std::cout << "Add control point ( mouse position =  " << x << " , " << y << " )" << std::endl;

            // Create Bezier curve while adding control points
            // Step 1: Add the current mouse position as a control point
            m_pMyBezier->addControlPoint(x, y);

            MyModel::PointLine controlPoint;
            controlPoint.position.x = x;
            controlPoint.position.y = y;
            controlPoint.position.z = 0.0f;
            controlPoint.color = glm::vec3(1.0f, 0.0f, 1.0f);

            m_vControlPointVertices.push_back(controlPoint);
        }

        // No more than 100 points
        if (m_vControlPointVertices.size() >= 100)
        {
            return;
        }

        // Step2: Create Bezier curve 
        if (m_vControlPointVertices.size() > 2)  // only create the curve if there is at least 3 points (2 points is just a line and 1 point is just a point)
        {
            m_pMyBezier->createBezierCurve(100);
        }

        // Step 3
        // Update the game objects for rendering
        for (auto& obj : m_vMyGameObjects)
        {
            if (obj.name() == std::string("control_points"))
            {
                obj.model->updatePointLines(m_vControlPointVertices);
            }
            else if (obj.name() == std::string("bezier_curve"))
            {
                obj.model->updatePointLines(m_pMyBezier->m_vCurve);
            }
        }
    }

    // If in edit mode and mouse button released -> end movement of selected control point if applicable
    else if (m_bCreateModel && !bMouseDown)
    {
        std::cout << "Mouse button released" << std::endl;
        m_bMoving = false; 
        index_of_selected_point = -1; 
    }
    
    // move camera
    else 
    {
        m_bMouseButtonPress = bMouseDown;
        m_myCamera.setButton(m_bMouseButtonPress, posx, posy);
    }
}

void MyApplication::mouseMotionEvent(float posx, float posy)
{
    // If in viewing mode, mouse movements are used to control camera position
    if (!m_bCreateModel)
	{
        m_myCamera.setMotion(m_bMouseButtonPress, posx, posy);
	}

    // Otherwise, in editting mode, mouse movements can be used to edit control point position
    else
    {
        if (!m_bMoving) return; 
        if (posy >= 0.5f) return; // cannot move points over the center line

        float x = 2.0f * posx - 1.0f;
        float y = -2.0f * posy + 1.0f;

        // If a point is currently selected, move it to desired coordinate
        if (index_of_selected_point != -1)
        {
            // Update position of selected control point
            m_vControlPointVertices[index_of_selected_point].position.x = x; 
            m_vControlPointVertices[index_of_selected_point].position.y = y; 
            m_pMyBezier->modifyControlPoint(index_of_selected_point, x, y); 

            m_pMyBezier->createBezierCurve(20); // recreate bezier curve based on modified control point

            // Update the corresponding models for rendering
            for (auto& obj : m_vMyGameObjects)
            {
                if (obj.name() == std::string("control_points"))
                {
                    obj.model->updatePointLines(m_vControlPointVertices);
                }
                else if (obj.name() == std::string("bezier_curve"))
                {
                    obj.model->updatePointLines(m_pMyBezier->m_vCurve);
                }
            }
        }
    }
}

void MyApplication::setCameraNavigationMode(MyCamera::MyCameraMode mode)
{
    m_myCamera.setMode(mode);   
}

void MyApplication::switchEditMode()
{
    // Switch between edit mode and camera mode
    m_bCreateModel = !m_bCreateModel;

    // If we are in edit mode, set the camera mode to orthographics mode
    m_bPerspectiveProjection = !m_bCreateModel;

    if (m_bPerspectiveProjection)
    {
	    std::cout << "Switch to Navigation Mode" << std::endl;
        setCameraNavigationMode(MyCamera::MYCAMERA_FITALL);
    }
    else
    {
	    std::cout << "Switch to Edit Mode" << std::endl;
        m_myCamera.setViewYXZ(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

// Clear everything (control points, bezier curves, surfaces, normal vectors) 
void MyApplication::resetSurface()
{
    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() == std::string("control_points"))
        {
            m_vControlPointVertices.clear();
            m_pMyBezier->clearControlPoints(); 
            obj.model->updatePointLines(m_vControlPointVertices);
        }
        else if (obj.name() == std::string("surface_normals"))
        {
            m_vNormalVectors.clear();
            obj.model->updatePointLines(m_vNormalVectors);
            m_bShowNormals = false; 
        }
        else if (obj.name() == std::string("bezier_curve"))
        {
            m_pMyBezier->m_vCurve.clear(); 
            obj.model->updatePointLines(m_pMyBezier->m_vCurve);
        }
        else if (obj.name() == std::string("bezier_surface"))
        {
            m_pMyBezier->m_vSurface.clear(); 
            m_pMyBezier->m_vIndices.clear(); 
            obj.model = nullptr; 
            m_bShowSurface = false; 
        }
    }
	std::cout << "Reset" << std::endl;
}

// Show or hide normal vectors 
void MyApplication::showHideNormalVectors()
{
    m_bShowNormals = !m_bShowNormals;

	if (m_bShowNormals)
	{
	    std::cout << "Show Normal Vectors" << std::endl;
	}
	else
	{
	    std::cout << "Hide Normal Vectors" << std::endl;
	}
}

// Show or hide surface to make editting control points easier
void MyApplication::showHideSurface()
{
    m_bShowSurface = !m_bShowSurface; 

    if (m_bShowSurface)
	{
	    std::cout << "Show Surface" << std::endl;
        createBezierRevolutionSurface(); 
	}
	else
	{
	    std::cout << "Hide Surface" << std::endl;
        m_bShowNormals = false; // If we are hiding surface, hide normal vectors as well
	}
}

// Revolve the computed bezier curve around the center line to form a hollow 3D model
void MyApplication::createBezierRevolutionSurface()
{
    std::cout << "Create Bezier Surface" << std::endl;
	
    m_vNormalVectors.clear();
    
	// Step 1: create the surface of revolution
	if (m_pMyBezier)
    {
        m_pMyBezier->createRevolutionSurface(100, 100);
    }

    // Step 2: Update the models of the game objects (surface and normals)

    // Create the surface model
    MyModel::Builder builder;

    builder.vertices = m_pMyBezier->m_vSurface;
    builder.indices = m_pMyBezier->m_vIndices;
    std::shared_ptr<MyModel> mysurface = std::make_shared<MyModel>(m_myDevice, builder);

    // Create the normal vectors model
	int nvertices = (int)m_pMyBezier->m_vSurface.size();
    m_vNormalVectors.resize(nvertices * 2);

    int index = 0;
    for (int ii = 0; ii < nvertices; ii++)
    {
        m_vNormalVectors[index].color = glm::vec3(0.0f, 1.0f, 0.0f);
        m_vNormalVectors[index++].position = m_pMyBezier->m_vSurface[ii].position;
        m_vNormalVectors[index].color = glm::vec3(0.0f, 1.0f, 0.0f);
        m_vNormalVectors[index++].position = m_pMyBezier->m_vSurface[ii].position + m_pMyBezier->m_vSurface[ii].normal * 0.1f;
    }

    std::shared_ptr<MyModel> mynormals = std::make_shared<MyModel>(m_myDevice, nvertices * 2);
    mynormals->updatePointLines(m_vNormalVectors);

    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.name() == std::string("bezier_surface"))
        {
            obj.model = mysurface;
        }
        else if (obj.name() == std::string("surface_normals"))
        {
            obj.model = mynormals;
        }
    }

	// Step 3: Update min and max for MyCamera from the surface and the normals
    float min_x, min_y, min_z;
    min_x = min_y = min_z = std::numeric_limits<float>::max();

    float max_x, max_y, max_z;
    max_x = max_y = max_z = std::numeric_limits<float>::min();

    for (auto & vertex : m_pMyBezier->m_vSurface)
    {
        min_x = std::min(vertex.position.x, min_x);
        min_y = std::min(vertex.position.y, min_y);
        min_z = std::min(vertex.position.z, min_z);

        max_x = std::max(vertex.position.x, max_x);
        max_y = std::max(vertex.position.y, max_y);
        max_z = std::max(vertex.position.z, max_z);

        if (m_bShowNormals) // Only consider normals if normals are currently being shown
        {
            min_x = std::min(vertex.normal.x, min_x);
            min_y = std::min(vertex.normal.y, min_y);
            min_z = std::min(vertex.normal.z, min_z);

            max_x = std::max(vertex.normal.x, max_x);
            max_y = std::max(vertex.normal.y, max_y);
            max_z = std::max(vertex.normal.z, max_z);
        }
    }
    glm::vec3 min(min_x, min_y, min_z); 
    glm::vec3 max(max_x, max_y, max_z); 

    m_myCamera.setSceneMinMax(min, max); 
}

