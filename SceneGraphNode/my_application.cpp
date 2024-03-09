#include "my_application.h"

// Render system
#include "my_simple_render_system.h"
#include "my_camera.h"
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

MyApplication::MyApplication() :
    m_bMoveCamera(true),
    m_bPerspectiveProjection(true)
{
    _loadGameObjects();
}

MyApplication::~MyApplication()
{

}

void MyApplication::run() 
{
    static bool bPerspectiveCamera = true;
 
    m_myWindow.bindMyApplication(this);
    MySimpleRenderSystem simpleRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass() };
    MyCamera camera{};

    // Empty object
    auto viewerObject = MyGameObject::createGameObject("CameraObject");
    MyKeyboardController cameraController{};

    viewerObject.transform.translation = glm::vec3(0.0f, 0.0f, 10.0f);

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

        if (m_bMoveCamera)
        {
            // Move camera
            cameraController.moveInPlaneXZ(m_myWindow.glfwWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
        }

        float apsectRatio = m_myRenderer.aspectRatio();

        // Put it here because the viewport may change
        if (m_bPerspectiveProjection)
            // near and far will automatically apply negative values
            camera.setPerspectiveProjection(glm::radians(50.f), apsectRatio, 0.1f, 100.f);
        else
            // because Y is down by default for Vulkan, when we set top to be minus value, we can flip the coordinate
            // such that Y is up. Because we move the part 2.5 units, the near and far value needs to cover the model
            // Also, near and far will automatically apply negative values
            camera.setOrthographicProjection(-apsectRatio * 5.0f, apsectRatio * 5.0f, -5.0f, 5.0f, -10.0f, 10.0f); 

        // Please note that commandBuffer could be null pointer
        // if the swapChain needs to be recreated
        if (auto commandBuffer = m_myRenderer.beginFrame())
		{
            // In case we have multiple render passes for the current frame
            // begin offsreen shadow pass
            // render shadow casting objects
            // end offscreen shadow pass

            m_myRenderer.beginSwapChainRenderPass(commandBuffer);
            
            simpleRenderSystem.renderSceneGraph(commandBuffer, m_pMySeceneGraphRoot, camera);

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

    if (m_bPerspectiveProjection)
        std::cout << "Switch to perspective camera mode" << std::endl;
    else
        std::cout << "Switch to orthographic camera mode" << std::endl;
}

void MyApplication::_loadGameObjects()
{
    std::vector<MyModel::Vertex> vertices
	{
      // left face (white)
      {{-.5f, -.46f, -.46f}, {.9f, .9f, .9f}},
      {{-.5f, .46f, .46f}, {.9f, .9f, .9f}},
      {{-.5f, -.46f, .46f}, {.9f, .9f, .9f}},
      {{-.5f, -.46f, -.46f}, {.9f, .9f, .9f}},
      {{-.5f, .46f, -.46f}, {.9f, .9f, .9f}},
      {{-.5f, .46f, .46f}, {.9f, .9f, .9f}},

      // borders
      {{-.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, -.5f}, {.0f, .0f, .0f}},

      {{-.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, .5f}, {.0f, .0f, .0f}},

      {{-.5f, -.46f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, .46f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, .46f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, .46f}, {.0f, .0f, .0f}},
      
      {{-.5f, .46f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, -.46f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.46f, -.46f}, {.0f, .0f, .0f}},
      {{-.5f, .46f, -.46f}, {.0f, .0f, .0f}},

      // right face (yellow)
      {{.5f, -.46f, -.46f}, {.8f, .8f, .1f}},
      {{.5f, .46f, .46f}, {.8f, .8f, .1f}},
      {{.5f, -.46f, .46f}, {.8f, .8f, .1f}},
      {{.5f, -.46f, -.46f}, {.8f, .8f, .1f}},
      {{.5f, .46f, -.46f}, {.8f, .8f, .1f}},
      {{.5f, .46f, .46f}, {.8f, .8f, .1f}},

      // borders
      {{.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, .46f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, .46f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .46f, -.5f}, {.0f, .0f, .0f}},

      {{.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, .5f}, {.0f, .0f, .0f}},

      {{.5f, -.46f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .46f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .46f, .46f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .46f, .46f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, .46f}, {.0f, .0f, .0f}},
      
      {{.5f, .46f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, -.46f}, {.0f, .0f, .0f}},
      {{.5f, .46f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.46f, -.46f}, {.0f, .0f, .0f}},
      {{.5f, .46f, -.46f}, {.0f, .0f, .0f}},

      // top face (orange, remember y axis points down)
      {{-.46f, -.5f, -.46f}, {.9f, .6f, .1f}},
      {{.46f, -.5f, .46f}, {.9f, .6f, .1f}},
      {{-.46f, -.5f, .46f}, {.9f, .6f, .1f}},
      {{-.46f, -.5f, -.46f}, {.9f, .6f, .1f}},
      {{.46f, -.5f, -.46f}, {.9f, .6f, .1f}},
      {{.46f, -.5f, .46f}, {.9f, .6f, .1f}},

      // borders
      {{.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, -.5f}, {.0f, .0f, .0f}},

      {{-.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, .5f}, {.0f, .0f, .0f}},

      {{-.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .46f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .46f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, .46f}, {.0f, .0f, .0f}},
      
      {{.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.46f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.46f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, -.46f}, {.0f, .0f, .0f}},

      // bottom face (red)
      {{-.46f, .5f, -.46f}, {.8f, .1f, .1f}},
      {{.46f, .5f, .46f}, {.8f, .1f, .1f}},
      {{-.46f, .5f, .46f}, {.8f, .1f, .1f}},
      {{-.46f, .5f, -.46f}, {.8f, .1f, .1f}},
      {{.46f, .5f, -.46f}, {.8f, .1f, .1f}},
      {{.46f, .5f, .46f}, {.8f, .1f, .1f}},

      // borders
      {{-.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, .5f}, {.0f, .0f, .0f}},

      {{.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, -.5f}, {.0f, .0f, .0f}},

      {{.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.46f}, {.0f, .0f, .0f}},
      {{.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.46f}, {.0f, .0f, .0f}},
      {{.46f, .5f, -.46f}, {.0f, .0f, .0f}},
      
      {{-.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .46f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .46f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, .46f}, {.0f, .0f, .0f}},

      // front face (blue)
      {{-.46f, -.46f, .5f}, {.1f, .1f, .8f}},
      {{.46f, .46f, .5f}, {.1f, .1f, .8f}},
      {{-.46f, .46f, .5f}, {.1f, .1f, .8f}},
      {{-.46f, -.46f, .5f}, {.1f, .1f, .8f}},
      {{.46f, -.46f, .5f}, {.1f, .1f, .8f}},
      {{.46f, .46f, .5f}, {.1f, .1f, .8f}},

      // borders
      {{-.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, .5f}, {.0f, .0f, .0f}},

      {{.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.5f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .5f}, {.0f, .0f, .0f}},

      {{.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.46f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, -.46f, .5f}, {.0f, .0f, .0f}},
      {{.46f, -.46f, .5f}, {.0f, .0f, .0f}},
      
      {{-.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .46f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, .5f}, {.0f, .0f, .0f}},
      {{.46f, .46f, .5f}, {.0f, .0f, .0f}},
      {{-.46f, .46f, .5f}, {.0f, .0f, .0f}},

      // back face (green)
      {{-.46f, -.46f, -.5f}, {.1f, .8f, .1f}},
      {{.46f, .46f, -.5f}, {.1f, .8f, .1f}},
      {{-.46f, .46f, -.5f}, {.1f, .8f, .1f}},
      {{-.46f, -.46f, -.5f}, {.1f, .8f, .1f}},
      {{.46f, -.46f, -.5f}, {.1f, .8f, .1f}},
      {{.46f, .46f, -.5f}, {.1f, .8f, .1f}},

      // borders
      {{.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, -.5f}, {.0f, .0f, .0f}},

      {{-.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.5f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.5f}, {.0f, .0f, .0f}},

      {{-.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .46f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .5f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, .46f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, .46f, -.5f}, {.0f, .0f, .0f}},
      
      {{.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, -.5f, -.5f}, {.0f, .0f, .0f}},
      {{-.46f, -.46f, -.5f}, {.0f, .0f, .0f}},
      {{.46f, -.46f, -.5f}, {.0f, .0f, .0f}},
    };

    auto cube_model = std::make_shared<MyModel>(m_myDevice, vertices); // all cubes will use this model

    m_pMySeceneGraphRoot = std::make_unique<MySceneGraphNode>("Root", 0); 
    m_pCurrentSceneGraphNode = m_pMySeceneGraphRoot.get(); 
    m_pMySeceneGraphRoot->m_bIsCurrent = true; 
    m_pMySeceneGraphRoot->m_bVisited = true; 

    std::shared_ptr<MySceneGraphNode> group1 = std::make_shared<MySceneGraphNode>("Group1", 1);
    std::shared_ptr<MySceneGraphNode> group2 = std::make_shared<MySceneGraphNode>("Group2", 2);

    // set transform for group1 and 2
    group1->transform.translation.x = -2.0f; 
    group2->transform.translation.x = 2.0f; 

    m_pMySeceneGraphRoot->addChild(group1);
    m_pMySeceneGraphRoot->addChild(group2);

    std::shared_ptr<MySceneGraphNode> cube1 = std::make_shared<MySceneGraphNode>("Cube1", 3);
    std::shared_ptr<MySceneGraphNode> cube2 = std::make_shared<MySceneGraphNode>("Cube2", 4);

    cube1->model = cube_model; 
    cube2->model = cube_model; 

    // set transform for cubes 1 and 2
    cube1->transform.translation.x = -1.0f; 
    cube2->transform.translation.x = 1.0f;

    group1->addChild(cube1);
    group1->addChild(cube2);

    std::shared_ptr<MySceneGraphNode> cube3 = std::make_shared<MySceneGraphNode>("Cube3", 5);
    std::shared_ptr<MySceneGraphNode> cube4 = std::make_shared<MySceneGraphNode>("Cube4", 6);

    cube3->model = cube_model; 
    cube4->model = cube_model; 

    // set transform for cubes 3 and 4
    cube3->transform.translation.x = -1.0f; 
    cube4->transform.translation.x = 1.0f;

    group2->addChild(cube3);
    group2->addChild(cube4);
}

void MyApplication::switchInteractionMode()
{
    m_bMoveCamera = !m_bMoveCamera;

    if (m_bMoveCamera)
        std::cout << "Move camera" << std::endl;
    else
        std::cout << "Move current scene graph node" << std::endl;
}

void MyApplication::handleMovementOfCurrentNode(MyAppKeyMap key)
{
    if (m_bMoveCamera)
    {
        return; 
    }
    if (key == KEY_LEFT)  
    {
        std::cout << "Move left"  << std::endl;
        m_pCurrentSceneGraphNode->transform.translation.x -= 0.01f; 
    }
    else if (key == KEY_RIGHT)
    {
        std::cout << "Move right" << std::endl;
        m_pCurrentSceneGraphNode->transform.translation.x += 0.01f; 
    }
    else if (key == KEY_UP) 
    {   
        std::cout << "Move up"    << std::endl;
        m_pCurrentSceneGraphNode->transform.translation.y += 0.01f; 
    }
    else if (key == KEY_DOWN)  
    {
        std::cout << "Move down"  << std::endl;
        m_pCurrentSceneGraphNode->transform.translation.y -= 0.01f; 
    }
    else if (key == KEY_ROTATE_X)
    {
        std::cout << "Rotate in X-direction"  << std::endl;
        m_pCurrentSceneGraphNode->transform.rotation.x += 0.01f; 
        m_pCurrentSceneGraphNode->transform.rotation.x = glm::mod(m_pCurrentSceneGraphNode->transform.rotation.x + 0.01f, glm::two_pi<float>());
    }
    else if (key == KEY_ROTATE_Y)
    {
        std::cout << "Rotate in Y-direction"  << std::endl;
        m_pCurrentSceneGraphNode->transform.rotation.y = glm::mod(m_pCurrentSceneGraphNode->transform.rotation.y + 0.01f, glm::two_pi<float>()); 
    }
    else if (key == KEY_ROTATE_Z)
    {
        std::cout << "Rotate in Z-direction"  << std::endl;
        m_pCurrentSceneGraphNode->transform.rotation.z = glm::mod(m_pCurrentSceneGraphNode->transform.rotation.z + 0.01f, glm::two_pi<float>()); 
    }
}

void MyApplication::printSceneGraph()
{
    std::cout << "Print scene graph" << std::endl;
    
    if (m_pMySeceneGraphRoot)
    {
        m_pMySeceneGraphRoot->printSceneGraph();
    }
}

void MyApplication::traverseNext()
{
    std::cout << "Traverse to the next node in the scene graph" << std::endl;

    if (m_pMySeceneGraphRoot)
    {    
        m_pCurrentSceneGraphNode = m_pMySeceneGraphRoot->traverseNext(m_pCurrentSceneGraphNode);
        m_pMySeceneGraphRoot->printSceneGraph();
    }
}

