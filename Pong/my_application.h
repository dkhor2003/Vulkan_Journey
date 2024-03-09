#ifndef __MY_APPLICATION_H__
#define __MY_APPLICATION_H__

#include "my_window.h"
#include "my_device.h"
#include "my_renderer.h"
#include "my_game_object.h"

#include <memory>
#include <vector>

class MyApplication 
{
public:
	static constexpr int WIDTH = 1000;
	static constexpr int HEIGHT = 1000;

	MyApplication();
	~MyApplication();

	MyApplication(const MyApplication&) = delete;
	MyApplication& operator=(const MyApplication&) = delete;

	void run();
	void movePaddle(int index, bool moveLeftOrRight);
	void resetGame();

private:
    void _loadGameObjects();
	void _updateGameLogic();

	MyWindow                  m_myWindow{ WIDTH, HEIGHT, "Assignment 2" };
	MyDevice                  m_myDevice{ m_myWindow };
	MyRenderer                m_myRenderer{ m_myWindow, m_myDevice };

	std::vector<MyGameObject> m_vMyGameObjects;
	float m_speed_multiplier = 1.0f; 
	float m_paddle_speed = 0.01f; 
};

#endif

