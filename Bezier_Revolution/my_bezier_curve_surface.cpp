#include "my_bezier_curve_surface.h"
#define M_PI        3.14159265358979323846264338327950288   /* pi */

void MyBezier::addControlPoint(float x, float y)
{
	m_vControlPoints.push_back(glm::vec2(x, y));
}

int MyBezier::numberOfControlPoints()
{
	return (int)m_vControlPoints.size();
}

void MyBezier::controlPoint(int index, glm::vec2& point)
{
	if (index >= 0 && index < m_vControlPoints.size())
	{
		point = m_vControlPoints[index];
	}
}

// A function used to clear the elements in private member data m_vControlPoints
void MyBezier::clearControlPoints()
{
    m_vControlPoints.clear(); 
}

void MyBezier::createBezierCurve(int resolution)
{
    m_vCurve.clear();

	// Step 1: identify the degree based on the number of control points m_vControlPoints
    int degree = (int)m_vControlPoints.size() - 1;

    glm::vec2 point; 

	// Step 2: use a for loop to loop through u from 0 to 1 based on the resolution
	for (int i = 0; i <= resolution; i++)
    {
        float u = 1.0f / resolution * i;

        // Step 3: use the funciton _pointOnBezierCurve to compute the point based on u
        _pointOnBezierCurve(degree, u, point); 

        MyModel::PointLine curve_point;
        curve_point.position.x = point.x;
        curve_point.position.y = point.y;
        curve_point.position.z = 0.0f;
        curve_point.color = glm::vec3(1.0f, 0.0f, 1.0f);

        // Step 4: add the computed point for u to m_vCurve
        m_vCurve.push_back(curve_point);
    }	
}

// Modify the coordinates of the control point at the given index in m_vControlPoints with the given (x, y)
void MyBezier::modifyControlPoint(int index, float x, float y)
{
    if (index < 0 || index >= m_vControlPoints.size()) return; // Check if given index is out of range
    m_vControlPoints[index].x = x; 
    m_vControlPoints[index].y = y; 
}

void MyBezier::createRevolutionSurface(int xResolution, int rResolution)
{
    m_vSurface.clear();
    m_vIndices.clear();

	// Step 1: deteremine the degree
	int degree = (int)m_vControlPoints.size() - 1;
	
	// Step 2: use two for loops
	// The first (outer) for loop is used to loop through xResolution (u), and the second
	// for loop (inner) for loop is used to loop through rResolution (r)
	for (int i = 0; i <= xResolution; i++)
    {
        float u = 1.0f / xResolution * i;

        // Step 3: in the outer for loop, compute the point on the Bezier curve using _pointOnBezierCurve
        glm::vec2 point, tangent;
        _pointOnBezierCurve(degree, u, point);

        // Step 4: in the outer for loop, compute the tangent vector using _derivative
	    //         use Perp formula to change the tangent vector into normal vector
        _derivative(degree, u, tangent);
        tangent = glm::normalize(tangent);

        // Vector (x, y) -> Perp Vector (-y, x);

        float deltaTheta = 2 * M_PI / rResolution;
        for (int j = 0; j < rResolution; j++)
        {
		    // Step 5: in the inner for loop, compute the vertex by rotate rangle

	        float cosTheta, sinTheta;

            // Compute cosTheta and sinTheta based on j
            cosTheta = glm::cos(deltaTheta * j);
            sinTheta = glm::sin(deltaTheta * j);

            // Compute the vertex based on the cosTheta and sinTheta
            MyModel::Vertex vertex;
            vertex.position = glm::vec3(point.x, point.y * cosTheta, point.y * sinTheta);
            vertex.normal = glm::vec3(-tangent.y, tangent.x * cosTheta, tangent.x * sinTheta);
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f); 
            vertex.uv = glm::vec2(0.0f, 0.0f);
            m_vSurface.push_back(vertex);

            // Step 6: in the inner for loop, compute the indices
            // Build indices and push the indices to m_vIndices, which will be passed to the index buffer for rendering

            if (i < xResolution)
            {
                uint32_t index = rResolution * i + j;
                if (j == rResolution - 1)
                {
                    m_vIndices.push_back(index);
                    m_vIndices.push_back(index + rResolution);
                    m_vIndices.push_back((i + 1) * rResolution);

                    m_vIndices.push_back(index);
                    m_vIndices.push_back(i * rResolution);
                    m_vIndices.push_back((i + 1) * rResolution);
                }
                else
                {
                    m_vIndices.push_back(index);
                    m_vIndices.push_back(index + rResolution);
                    m_vIndices.push_back(index + rResolution + 1);

                    m_vIndices.push_back(index);
                    m_vIndices.push_back(index + 1);
                    m_vIndices.push_back(index + rResolution + 1);
                }
            }
        }
    }
}

void MyBezier::_allBernstein(int n, float u, float* B)
{
    /* Compute all n-th degree Bernstein Polynomials */
    /* Input: n, u */
    /* Output: B (an array, B[0], �K, B[n]) */
    int k = 0;
    float u1 = 1.0f - u;
    float saved = 0.0f;
    float temp = 0.0f;
    B[0] = 1.0;

    for (int j = 1; j <= n; j++)
    {
        saved = 0.0;
        for (k = 0; k < j; k++)
        {
            temp = B[k];
            B[k] = saved + u1 * temp;
            saved = u * temp;
        }

        B[j] = saved;
    }
}

void MyBezier::_pointOnBezierCurve(int degree, float u, glm::vec2 &point)
{
    /* Compute point on Bezier curve */
    /* Input: P (control point) n, u */
    /* Output: C (a point) */
    std::vector<float> vfB(degree + 1, 0.0f);

    _allBernstein(degree, u, vfB.data()); /* B is a local array */
    point = glm::vec3{ 0.0f };
    for (int k = 0; k <= degree; k++)
    {
        point = point + m_vControlPoints[k] * vfB[k];
    }
}

void MyBezier::_derivative(int degree, float u, glm::vec2& der)
{
    std::vector<float> vfB(degree, 0.0f);

    _allBernstein(degree - 1, u, vfB.data());
    der = glm::vec3{ 0.0f };
    for (int k = 0; k <= degree - 1; k++)
    {
        der = der +  (m_vControlPoints[k + 1] - m_vControlPoints[k]) * vfB[k];
    }

    der = der * (float)degree;
}

