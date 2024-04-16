#ifndef __MY_APPLICATION_H__
#define __MY_APPLICATION_H__

#include "my_window.h"
#include "my_device.h"
#include "my_renderer.h"
#include "my_game_object.h"
#include "my_camera.h"
#include "my_bezier_curve_surface.h"

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

	// Assignment
	void switchProjectionMatrix();
	void mouseButtonEvent(bool bMouseDown, float posx, float posy);
	void mouseMotionEvent(float posx, float posy);
	void setCameraNavigationMode(MyCamera::MyCameraMode mode);

	void switchEditMode();
	void resetSurface();
	void showHideNormalVectors();
	void showHideSurface();
	void createBezierRevolutionSurface();

private:
	void _loadGameObjects();
	int  _queryControlPoints(float posx, float posy); 

	MyWindow                        m_myWindow{ WIDTH, HEIGHT, "Bezier Revolution" };
	MyDevice                        m_myDevice{ m_myWindow };
	MyRenderer                      m_myRenderer{ m_myWindow, m_myDevice };

	std::vector<MyGameObject>       m_vMyGameObjects;
	MyCamera                        m_myCamera{};
	bool                            m_bPerspectiveProjection;
	bool                            m_bMouseButtonPress = false;
	bool                            m_bCreateModel = false;
	bool                            m_bShowNormals = false;
	bool                            m_bShowSurface =  false;

	std::vector<MyModel::PointLine> m_vControlPointVertices;
	std::vector<MyModel::PointLine> m_vNormalVectors;
	std::shared_ptr<MyBezier>       m_pMyBezier;

	int index_of_selected_point = -1; 
	bool         m_bMoving = false;

};

#endif

