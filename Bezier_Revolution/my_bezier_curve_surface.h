#ifndef __MY_BEZIER_CURVE_SURFACE_H__
#define __MY_BEZIER_CURVE_SURFACE_H__

#include <vector>
#include <glm/glm.hpp>
#include "my_model.h"

class MyBezier
{
public:
	void addControlPoint(float x, float y);
	int  numberOfControlPoints();
	void clearControlPoints(); 
	void controlPoint(int index, glm::vec2& point);
	void createBezierCurve(int resolution);
	void createRevolutionSurface(int xResolution, int rResolution);
	void modifyControlPoint(int index, float x, float y);

	std::vector<MyModel::PointLine> m_vCurve;
	std::vector<MyModel::Vertex>    m_vSurface;
	std::vector<uint32_t>           m_vIndices;

protected:

	void _allBernstein(int n, float u, float* B);
	void _pointOnBezierCurve(int n, float u, glm::vec2 &point);
	void _derivative(int degree, float u, glm::vec2& der);

	std::vector<glm::vec2>          m_vControlPoints;
	//float                           m_fB[100]; // Bernstein function
};


#endif

