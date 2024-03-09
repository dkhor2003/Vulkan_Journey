#include "my_application.h"

// Render system
#include "my_simple_render_system.h"

// use radian rather degree for angle
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Std
#include <stdexcept>
#include <array>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <windows.h>

MyApplication::MyApplication()
{
    _loadGameObjects();
}

MyApplication::~MyApplication()
{

}

void MyApplication::run() 
{
    MySimpleRenderSystem simpleRenderSystem{ m_myDevice, m_myRenderer.swapChainRenderPass() };

    m_myWindow.bindMyApplication(this);

    time_t start, end;
    double elapsed;
    float x, y; 
    int current_sec = 0; 

    time(&start); //start timer

    while (!m_myWindow.shouldClose()) 
    {
        time(&end); 
        elapsed = difftime(end, start); //calculate elapsed time in seconds. 

        if (int(elapsed) - current_sec == 1)
        {
            current_sec += 1;
            if (current_sec % 5 == 0) // speed of ball increases by 20% every 5 second
            {
                m_speed_multiplier *= 1.2f; 
                m_paddle_speed *= 1.05f;  // speed of paddle moving is increased by 5% to compensate for the increase in ball speed
            }
        }

        // Note: depending on the platforms (PC, Linux or Mac), this function
        // will cause the event proecssing to block during a Window move, resize or
        // menu operation. Users can use the "window refresh callback" to redraw the
        // contents of the window when necessary during such operation.
        m_myWindow.pollEvents();
		
        _updateGameLogic();

        // Please note that commandBuffer could be null pointer
        // if the swapChain needs to be recreated
        if (auto commandBuffer = m_myRenderer.beginFrame())
		{
            // In case we have multiple render passes for the current frame
            // begin offsreen shadow pass
            // render shadow casting objects
            // end offscreen shadow pass

            m_myRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, m_vMyGameObjects);
            m_myRenderer.endSwapChainRenderPass(commandBuffer);

            m_myRenderer.endFrame();
        }
    }

    // GPU will block until all CPU is complete
    vkDeviceWaitIdle(m_myDevice.device());
}

void MyApplication::_loadGameObjects()
{
    float half_width, top_y, bottom_y; 

    half_width = 0.3f; 
    top_y = -0.8f; 
    bottom_y = -0.7f; 

    std::vector<MyModel::Vertex> vertices1
	{
        {{half_width, top_y}, {1.0f, 0.0f, 0.0f}},
        {{half_width, bottom_y},  {1.0f, 0.0f, 0.0f}},
        {{-half_width, top_y}, {1.0f, 0.0f, 0.0f}},
        {{-half_width, bottom_y}, {1.0f, 0.0f, 0.0f}},
        {{half_width, bottom_y},  {1.0f, 0.0f, 0.0f}},
        {{-half_width, top_y}, {1.0f, 0.0f, 0.0f}}
    };

    auto model1 = std::make_shared<MyModel>(m_myDevice, vertices1);

    auto paddle1 = MyGameObject::createGameObject("paddle1");
    paddle1.half_width = half_width; 
    paddle1.top_y = top_y; 
    paddle1.bottom_y = bottom_y; 
    paddle1.model = model1;
    paddle1.transform2d.translation.x = 0.0f; 
    paddle1.transform2d.scale = {1.0f, 1.0f};
    paddle1.transform2d.rotation = 0.0f * glm::two_pi<float>(); // 90 degree rotation

    m_vMyGameObjects.push_back(std::move(paddle1));

////////////////////////////////////////////////////////////////////////////

    half_width = 0.3f; 
    top_y = 0.7f; 
    bottom_y = 0.8f; 

    std::vector<MyModel::Vertex> vertices2
	{
        {{half_width, bottom_y},  {0.0f, 0.0f, 1.0f}},
        {{half_width, top_y},  {0.0f, 0.0f, 1.0f}},
        {{-half_width, bottom_y}, {0.0f, 0.0f, 1.0f}},
        {{-half_width, top_y}, {0.0f, 0.0f, 1.0f}},
        {{half_width, top_y},  {0.0f, 0.0f, 1.0f}},
        {{-half_width, bottom_y}, {0.0f, 0.0f, 1.0f}}
    };

    auto model2 = std::make_shared<MyModel>(m_myDevice, vertices2);

    auto paddle2 = MyGameObject::createGameObject("paddle2");
    paddle2.half_width = half_width; 
    paddle2.top_y = top_y; 
    paddle2.bottom_y = bottom_y; 
    paddle2.model = model2;
    paddle2.transform2d.translation.x = 0.0f; 
    paddle2.transform2d.scale = {1.0f, 1.0f};
    paddle2.transform2d.rotation = 0.0f * glm::two_pi<float>(); // 90 degree rotation

    m_vMyGameObjects.push_back(std::move(paddle2));

////////////////////////////////////////////////////////////////////////////

    half_width = 0.05f; 
    top_y = -0.05f; 
    bottom_y = 0.05f; 

    std::vector<MyModel::Vertex> vertices3
	{
        {{half_width, bottom_y}, {0.0f, 1.0f, 1.0f}},
        {{half_width, top_y}, {0.0f, 1.0f, 1.0f}},
        {{0, 0}, {1.0f, 1.0f, 1.0f}},
        {{half_width, bottom_y}, {0.0f, 1.0f, 1.0f}},
        {{-half_width, bottom_y}, {0.0f, 1.0f, 1.0f}},
        {{0, 0}, {1.0f, 1.0f, 1.0f}},
        {{-half_width, bottom_y}, {0.0f, 1.0f, 1.0f}},
        {{-half_width, top_y}, {0.0f, 1.0f, 1.0f}},
        {{0, 0}, {1.0f, 1.0f, 1.0f}},
        {{-half_width, top_y}, {0.0f, 1.0f, 1.0f}},
        {{half_width, top_y}, {0.0f, 1.0f, 1.0f}},
        {{0, 0}, {1.0f, 1.0f, 1.0f}},
    };

    auto model3 = std::make_shared<MyModel>(m_myDevice, vertices3);

    auto ball = MyGameObject::createGameObject("ball");
    ball.half_width = half_width; 
    ball.top_y = top_y; 
    ball.bottom_y = bottom_y; 
    ball.model = model3;
    ball.transform2d.translation.x = 0.0f; 
    ball.transform2d.scale = {1.0f, 1.0f};
    ball.transform2d.rotation = 0.0f * glm::two_pi<float>(); // 90 degree rotation

    m_vMyGameObjects.push_back(std::move(ball));
}

void MyApplication::_updateGameLogic()
{
    MyPaddle* pPaddle1, * pPaddle2; 
    MyBall* pBall; 

    for (auto& obj : m_vMyGameObjects)
    {
        if (obj.getName() == "paddle1")
        {
            pPaddle1 = (MyPaddle*)&obj; 
        }
        else if (obj.getName() == "paddle2")
        {
            pPaddle2 = (MyPaddle*)&obj; 
        }
        else if (obj.getName() == "ball")
        {
            pBall = (MyBall*)&obj; 
        }
    }

    pBall -> UpdateGameLogic(pPaddle1, pPaddle2, m_speed_multiplier); 
    
}


void MyApplication::movePaddle(int index, bool moveLeftOrRight)
{
    // As per construction, the top paddle has index 0 and the bottom paddle has index 1
    MyPaddle* pPaddle = (MyPaddle*)&m_vMyGameObjects[index];
    pPaddle -> UpdateGameLogic(0.3f, moveLeftOrRight, m_paddle_speed); 

}

void MyApplication::resetGame()
{
    std::cout << "Reset game!" << std::endl;

    for (auto& obj : m_vMyGameObjects)
    {
        obj.setX(0.0f);    
        obj.setY(0.0f);   
    }

    m_speed_multiplier = 1.0f; 
    m_paddle_speed = 0.01f;
}

