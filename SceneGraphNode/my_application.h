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
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	enum MyAppKeyMap
	{
		KEY_NONE = 0,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_ROTATE_X, 
		KEY_ROTATE_Y, 
		KEY_ROTATE_Z
	};

	MyApplication();
	~MyApplication();

	MyApplication(const MyApplication&) = delete;
	MyApplication& operator=(const MyApplication&) = delete;

	void run();

	// Interaction
	void switchProjectionMatrix();
	void switchInteractionMode();
	void handleMovementOfCurrentNode(MyAppKeyMap key);
	void printSceneGraph();
	void traverseNext();

private:
	void _loadGameObjects();

	MyWindow                           m_myWindow{ WIDTH, HEIGHT, "Scene Graph Node" };
	MyDevice                           m_myDevice{ m_myWindow };
	MyRenderer                         m_myRenderer{ m_myWindow, m_myDevice };

	std::shared_ptr<MySceneGraphNode>  m_pMySeceneGraphRoot;               // Scene graph root node pointer
	bool                               m_bPerspectiveProjection;           // Switch between orthographic and perspective camera
	bool                               m_bMoveCamera;                      // Move camera or move game object(s)

	MySceneGraphNode*                  m_pCurrentSceneGraphNode = nullptr; // Pointer to the current node in the scene graph
};

#endif

