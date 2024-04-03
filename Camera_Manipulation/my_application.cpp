#include "my_application.h"

// Render system
#include "my_simple_render_system.h"
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
#include <algorithm>

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
    static bool bPerspectiveCamera = true;

    m_myWindow.bindMyApplication(this);
    MySimpleRenderSystem simpleRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass() };

    // Empty object
    auto viewerObject = MyGameObject::createGameObject();
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

        float apsectRatio = m_myRenderer.aspectRatio();

        // Put it here because the viewport may change
        if (m_bPerspectiveProjection)
            // near and far will automatically apply negative values
            m_myCamera.setPerspectiveProjection(glm::radians(50.f), apsectRatio, 0.1f, 100.f);
        else
            // because Y is down by default for Vulkan, when we set top to be minus value, we can flip the coordinate
            // such that Y is up. Because we move the part 2.5 units, the near and far value needs to cover the model
            // Also, near and far will automatically apply negative values
            m_myCamera.setOrthographicProjection(-apsectRatio * 3.0f, apsectRatio * 3.0f, -3.0f, 3.0f, -50.0f, 50.0f);
 
        // Please note that commandBuffer could be null pointer
        // if the swapChain needs to be recreated
        if (auto commandBuffer = m_myRenderer.beginFrame())
		{
            // In case we have multiple render passes for the current frame
            // begin offsreen shadow pass
            // render shadow casting objects
            // end offscreen shadow pass

            m_myRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, m_vMyGameObjects, m_myCamera);
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
    glm::vec3 cup_min, cup_max, teapot_max, teapot_min, cube_min, cube_max, min, max; 

    glm::vec4 affine_cup_min, affine_cup_max, affine_teapot_min, affine_teapot_max; 

    glm::mat4 cupMat, teapotMat; 

    // create cup model from obj file 
    std::shared_ptr<MyModel> cupModel = MyModel::createModelFromFile(m_myDevice, "models/Cup.obj", cup_min, cup_max);
    affine_cup_min = {cup_min, 1.0f}; 
    affine_cup_max = {cup_max, 1.0f}; 

    glm::vec3 cup_mid = (cup_min + cup_max) / 2.0f; 

    auto cup_obj = MyGameObject::createGameObject(); 

    cup_obj.model = cupModel; 
    cup_obj.transform.translation = { 3.0f, 0.0f, 0.0f }; // for perspective
    cup_obj.transform.scale = glm::vec3(0.2f);

    cupMat = cup_obj.transform.mat4();

    affine_cup_min = cupMat * affine_cup_min; 
    affine_cup_max = cupMat * affine_cup_max; 
    cup_min = {affine_cup_min.x, affine_cup_min.y, affine_cup_min.z};
    cup_max = {affine_cup_max.x, affine_cup_max.y, affine_cup_max.z};

    m_vMyGameObjects.push_back(std::move(cup_obj));

    // create cube model from obj file 
    std::shared_ptr<MyModel> cubeModel = MyModel::createModelFromFile(m_myDevice, "models/colored_cube.obj", cube_min, cube_max);
    auto cube_obj = MyGameObject::createGameObject(); 

    // same transformation as cup object but with smaller scaling to make the cube be inside the cup
    cube_obj.model = cubeModel; 
    cube_obj.transform.translation = { 3.0f, 0.5f, 0.0f }; // for perspective
    cube_obj.transform.scale = glm::vec3(0.1f);

    m_vMyGameObjects.push_back(std::move(cube_obj));

    // create teapot model from obj file 
    std::shared_ptr<MyModel> teapotModel = MyModel::createModelFromFile(m_myDevice, "models/teapot.obj", teapot_min, teapot_max);
    affine_teapot_min = {teapot_min, 1.0f}; 
    affine_teapot_max = {teapot_max, 1.0f}; 

    auto teapot_obj = MyGameObject::createGameObject(); 

    teapot_obj.model = teapotModel; 
    teapot_obj.transform.translation = { -1.5f, 2.0f, 0.0f }; // for perspective
    teapot_obj.transform.scale = glm::vec3(1.0f);
    float theta = -glm::pi<float>() / 4.0f; 
	teapot_obj.transform.rotation.z = theta; 

    teapotMat = teapot_obj.transform.mat4();
    affine_teapot_min = teapotMat * affine_teapot_min; 
    affine_teapot_max = teapotMat * affine_teapot_max; 
    teapot_min = {affine_teapot_min.x, affine_teapot_min.y, affine_teapot_min.z};
    teapot_max = {affine_teapot_max.x, affine_teapot_max.y, affine_teapot_max.z};

    m_vMyGameObjects.push_back(std::move(teapot_obj));

    // Calculate the overall scene's min and max
    for (int i = 0; i < 3; i++)
    {
        min[i] = std::min(teapot_min[i], cup_min[i]); 
        max[i] = std::max(teapot_max[i], cup_max[i]); 
    }
    
    // std::cout << "Min: " << min.x << ", " << min.y << ", " << min.z << std::endl;
    // std::cout << "Max: " << max.x << ", " << max.y << ", " << max.z << std::endl;

    // Offset both min and max's y-component so that initially when twisting the camera, the scene fits the window screen. 
    min.y += 1.2f;
    max.y += 1.2f; 

    m_myCamera.setSceneMinMax(min, max);
    setCameraNavigationMode(MyCamera::MYCAMERA_FITALL); // fit all to ensure object is at center of screen when program starts
}

void MyApplication::mouseButtonEvent(bool bMouseDown, float posx, float posy)
{
    m_bMouseButtonPress = bMouseDown;
    m_myCamera.setButton(m_bMouseButtonPress, posx, posy);
}

void MyApplication::mouseMotionEvent(float posx, float posy)
{
    m_myCamera.setMotion(m_bMouseButtonPress, posx, posy);
}

void MyApplication::setCameraNavigationMode(MyCamera::MyCameraMode mode)
{
    m_myCamera.setMode(mode);   
}
