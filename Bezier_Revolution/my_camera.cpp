#include "my_camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

// use radian rather degree for angle
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

// std
#include <cassert>
#include <limits>
#include <iostream>

#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */

void MyCamera::setOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far) 
{
    // Based on the equation from
    // https://www.songho.ca/opengl/gl_projectionmatrix.html
    //
    // Follow Vulkan's convention that X is right, Y is down and Z is into the screen

    m_m4ProjectionMatrix = glm::mat4{ 1.0f };

    //
    // glm matrix layout
    // 
    // [0,0] [1,0] [2,0] [3,0]
    // [0,1] [1,1] [2,1] [3,1]
    // [0,2] [1,2] [2,2] [3,2]
    // [0,3] [1,3] [2,3] [3,3]
    // 
    m_m4ProjectionMatrix[0][0] = 2.f / (right - left);
    m_m4ProjectionMatrix[1][1] = 2.f / (top - bottom);
    m_m4ProjectionMatrix[2][2] = -2.0f / (far - near);
    m_m4ProjectionMatrix[3][0] = -(right + left) / (right - left);
    m_m4ProjectionMatrix[3][1] = -(top + bottom) / (top - bottom);
    m_m4ProjectionMatrix[3][2] = -(far + near) / (far - near);
}

void MyCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
    assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

    // Use OpenGL matrix but follow Vulkan's convention that Y is down
    const float tanHalfFovy = tan(fovy / 2.f);

    // Based on the equation from
    // https://www.songho.ca/opengl/gl_projectionmatrix.html
    //
    // Follow Vulkan's convention that X is right, Y is down and Z is into the screen


    const float right = near * tanHalfFovy * aspect;  // Right is positive
    const float left = -1.0f * right;                 // Left is minus X (because near is a negative value)

    const float bottom = near * tanHalfFovy;          // bottom is positive (because Y is down)
    const float top = -1.0f * bottom;                 // top is negative

    //
    // Use this matrix such that it will become X - right, Y - up, Z - out of the screen
    //
    //
    // glm matrix layout
    // 
    // [0,0] [1,0] [2,0] [3,0]
    // [0,1] [1,1] [2,1] [3,1]
    // [0,2] [1,2] [2,2] [3,2]
    // [0,3] [1,3] [2,3] [3,3]
    // 
    m_m4ProjectionMatrix = glm::mat4{ 0.0f };
    m_m4ProjectionMatrix[0][0] = 2.0f * near / (right - left);
    m_m4ProjectionMatrix[2][0] = (right + left) / (right - left);
    m_m4ProjectionMatrix[1][1] = 2.0f * near / (top - bottom);
    m_m4ProjectionMatrix[2][1] = (top + bottom) / (top - bottom);
    m_m4ProjectionMatrix[2][2] = -1.0f * (far + near) / (far - near);
    m_m4ProjectionMatrix[3][2] = -2.0f * far * near / (far - near);
    m_m4ProjectionMatrix[2][3] = -1.0f;
}

void MyCamera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    //
    // glm matrix layout
    // 
    // [0,0] [1,0] [2,0] [3,0]
    // [0,1] [1,1] [2,1] [3,1]
    // [0,2] [1,2] [2,2] [3,2]
    // [0,3] [1,3] [2,3] [3,3]
    // 
    // If using Y-down coordinate system
    const glm::vec3 w{ glm::normalize(direction)}; // Z is out of the screen
    const glm::vec3 u{ glm::normalize(glm::cross(up, w)) };
    const glm::vec3 v{ glm::cross(w, u) };

    // View to world
    m_m4ViewMatrix = glm::mat4{ 1.f };
    m_m4ViewMatrix[0][0] = u.x;
    m_m4ViewMatrix[1][0] = u.y;
    m_m4ViewMatrix[2][0] = u.z;
    m_m4ViewMatrix[0][1] = v.x;
    m_m4ViewMatrix[1][1] = v.y;
    m_m4ViewMatrix[2][1] = v.z;
    m_m4ViewMatrix[0][2] = w.x;
    m_m4ViewMatrix[1][2] = w.y;
    m_m4ViewMatrix[2][2] = w.z;
    m_m4ViewMatrix[3][0] = position.x;
    m_m4ViewMatrix[3][1] = position.y;
    m_m4ViewMatrix[3][2] = position.z;

    // World to view
    m_m4ViewMatrix = glm::inverse(m_m4ViewMatrix);
}

void MyCamera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    // Z direction is out of the screen so we need to flip the view direction
    setViewDirection(position, position - target, up);
}

void MyCamera::setViewYXZ(glm::vec3 position, glm::vec3 rotation)
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
    const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
    const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };

    m_m4ViewMatrix = glm::mat4{ 1.f };
    m_m4ViewMatrix[0][0] = u.x;
    m_m4ViewMatrix[1][0] = u.y;
    m_m4ViewMatrix[2][0] = u.z;
    m_m4ViewMatrix[0][1] = v.x;
    m_m4ViewMatrix[1][1] = v.y;
    m_m4ViewMatrix[2][1] = v.z;
    m_m4ViewMatrix[0][2] = w.x;
    m_m4ViewMatrix[1][2] = w.y;
    m_m4ViewMatrix[2][2] = w.z;
    m_m4ViewMatrix[3][0] = -glm::dot(u, position);
    m_m4ViewMatrix[3][1] = -glm::dot(v, position);
    m_m4ViewMatrix[3][2] = -glm::dot(w, position);
}

void MyCamera::setMode(MyCameraMode mode)
{
    m_eMode = mode;

    std::cout << "MyCamera::setMode = " << (int)mode << std::endl;

    // Note: fit all doesn't require mouse operation
    if (m_eMode == MYCAMERA_FITALL)
    {
        std::cout << "Fit All" << std::endl;
        _fitAll();
    }
}

void MyCamera::setSceneMinMax(glm::vec3 min, glm::vec3 max)
{
    m_vSceneMin = min;
    m_vSceneMax = max;
}

void MyCamera::setButton(bool buttonPress, float x, float y)
{
    // set the temp tranform to identity matrix
    m_m4TempTransform = glm::mat4{ 1.f };

    if (buttonPress)
    {
        std::cout << "Mouse button pressed" << std::endl;
        m_vCurrPos.x = x;
        m_vCurrPos.y = y;
        m_vPrevPos = m_vCurrPos;
        m_bMoving = true;
    }
    else
    {
        std::cout << "Mouse button released" << std::endl;
        m_bMoving = false;
    }
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
    os << std::setprecision(2) << std::fixed;
    for (int i = 0; i < mat.length(); i++)
    {
        os << mat[0][i] << ", " << mat[1][i] << ", " << mat[2][i] << ", " << mat[3][i] << "\n";
    }
    os << "-------------------------------------------\n"; 
    return os;
};

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
    os << vec[0] << ", " << vec[1] << ", " << vec[2] << "\n";
    return os;
};

void MyCamera::setMotion(bool buttonPress, float x, float y)
{
    if (!m_bMoving)
        return;

    m_vCurrPos.x = x;
    m_vCurrPos.y = y;

    glm::vec2 delta = m_vCurrPos - m_vPrevPos; // Change in mouse position
    float mag = glm::length(delta);

    if (mag < 1.0e-6f) return; // Not significant mouse movement for transformation on object

    if (m_eMode == MYCAMERA_ROTATE) // Press key R
    {
        std::cout << "  Rotating..." << std::endl;
        _rotate(delta.x, delta.y);
    }
    else if (m_eMode == MYCAMERA_PAN) // Press key P
    {
        std::cout << "  Panning..." << std::endl;
        _pan(delta.x, delta.y);
    }
    else if (m_eMode == MYCAMERA_ZOOM) // Press key Z
    {
        std::cout << "  Zooming..." << std::endl;
        _zoom(delta.x, delta.y);
    }
    else if (m_eMode == MYCAMERA_TWIST) // Press key T
    {
        std::cout << "  Twisting..." << std::endl;
        _twist(delta.x, delta.y);
    }

    glm::vec3 mid = (m_vSceneMax + m_vSceneMin) / 2.0f;  // Mid of scene

    glm::mat4 offset_mat = {
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        {-mid, 1}
    };

    // std::cout << "Offset Matrix:" << std::endl; 
    // std::cout << offset_mat; 

    glm::mat4 inverse_offset_mat = {
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        {mid, 1}
    };

    // std::cout << "Inverse Offset Matrix:" << std::endl; 
    // std::cout << inverse_offset_mat; 

    glm::mat4 M_TT = { // Translation component of m_m4TempTransform
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f}, 
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        m_m4TempTransform[3]
    };

    // std::cout << "Temporary Transform Matrix Translation Components:" << std::endl; 
    // std::cout << M_TT; 

    glm::mat4 M_TR = { // Rotation component of m_m4TempTransform
        m_m4TempTransform[0],
        m_m4TempTransform[1],
        m_m4TempTransform[2],
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    // std::cout << "Temporary Transform Matrix Rotation Components:" << std::endl; 
    // std::cout << M_TR; 
    // std::cout << "##############################################" << std::endl; 

    // First offset the object to the origin and apply the rotation components in the temporary transform matrix. 
    // Then, offset the object back to its original position and apply the original transformation matrix as well 
    // as the translation components of the temporary transform matrix
    m_m4ViewMatrix = M_TT * m_m4ViewMatrix * inverse_offset_mat * M_TR * offset_mat; 

	// Set the current mouse position as the previous one
	m_vPrevPos = m_vCurrPos;
}

void MyCamera::_pan(float dx, float dy)
{
    glm::vec3 mid = (m_vSceneMax + m_vSceneMin) / 2.0f; // Midpoint of scene 

    float radius = glm::length(m_vSceneMax - mid); // radius of scene

    float diameter = 2 * radius; // diameter of the scene

    // Change in x and y position based on mouse movement
    float delta_x = diameter * dx;
    float delta_y = diameter * dy;

    m_m4TempTransform = glm::mat4{ 1.0f };
    m_m4TempTransform[3][0] = delta_x;
    m_m4TempTransform[3][1] = -delta_y; // need to multiply by -1 since we flipped the y-axis upside down when setting projection matrix (Vulkan by default has positive y pointing down)
}

void MyCamera::_zoom(float dx, float dy)
{
    // Dragging mouse in positive y-direction will result in smaller m-value --> new view distance will be smaller than before --> camera more closer to object
    // Dragging mouse in negative y-direction will result in larger m-value  --> new view distance will be larger than before  --> camera more further from object

    // positive y is up and negative y is down since we set it as such in the projection matrix of the camera
    float m = 1.0f / (1.0f + dy * 2.0f);

    glm::vec3 mid = (m_vSceneMax + m_vSceneMin) / 2.0f; // Midpoint of Scene 

    float radius = glm::length(m_vSceneMax - mid); // radius of scene to be looked at

    float curr_view_dist = radius / (glm::tan(glm::radians(25.f))); // viewing distance that is needed to look at all scene

    float new_view_dist = curr_view_dist * m; 

    m_m4TempTransform[3][2] = new_view_dist - curr_view_dist; // set the change in view distance in temporary transform matrix
}

void MyCamera::_rotate(float dx, float dy)
{
    glm::vec3 sx, sy, sz, axis, tb_axis;
    float mag = sqrtf(dx * dx + dy * dy);
    float tb_angle = mag * 100.0f; // Scaling factor for the speed of rotation

    axis.x = dy;
    axis.y = dx;
    axis.z = 0.0f;

    _getScreenXYZ(sx, sy, sz);

    tb_axis = sx * axis.x + sy * axis.y + sz * axis.z;

    _atRotate(tb_axis.x, tb_axis.y, tb_axis.z, tb_angle);
}

void MyCamera::_atRotate(float x, float y, float z, float angle)
{
    int i = 0;
    float rad = angle * glm::pi<float>() / 180.0f; // Become radius
    float cosAng = (float)(cos(rad));
    float sinAng = (float)(sin(rad));

    glm::vec3 axis(x, y, z);
    axis = glm::normalize(axis);

    // Reset delta matrix
    m_m4TempTransform = glm::mat4(0.0f);

    float x2 = axis.x * axis.x;
    float xy = axis.x * axis.y;
    float xz = axis.x * axis.z;

    float y2 = axis.y * axis.y;
    float yz = axis.y * axis.z;
    float z2 = axis.z * axis.z;

    // Arbitrary Rotation - Matrix Form
    m_m4TempTransform[0][0] = x2 + (cosAng * (1 - x2));
    m_m4TempTransform[0][1] = xy - (cosAng * xy) + (sinAng * axis[2]);
    m_m4TempTransform[0][2] = xz - (cosAng * xz) - (sinAng * axis[1]);

    m_m4TempTransform[1][0] = xy - (cosAng * xy) - (sinAng * axis[2]);
    m_m4TempTransform[1][1] = y2 + (cosAng * (1 - y2));
    m_m4TempTransform[1][2] = yz - (cosAng * yz) + (sinAng * axis[0]);

    m_m4TempTransform[2][0] = xz - (cosAng * xz) + (sinAng * axis[1]);
    m_m4TempTransform[2][1] = yz - (cosAng * yz) - (sinAng * axis[0]);
    m_m4TempTransform[2][2] = z2 + (cosAng * (1 - z2));

    m_m4TempTransform[3][3] = 1.0f;
}

void MyCamera::_twist(float dx, float dy)
{
    glm::vec3 sx, sy, sz;
    _getScreenXYZ(sx, sy, sz);

    float delta_theta = 0.0f;

    float prev_theta = atan2f(m_vPrevPos.x - 0.5, m_vPrevPos.y - 0.5);

    float curr_theta = atan2f(m_vCurrPos.x - 0.5, m_vCurrPos.y - 0.5);

    delta_theta = (180 / M_PI) * (curr_theta - prev_theta);

    _atRotate(sz.x, sz.y, sz.z, delta_theta);
}

void MyCamera::_fitAll()
{
	glm::vec3 sx, sy, sz; 

    // Reset delta matrix first
    m_m4TempTransform = glm::mat4(1.0f);

    glm::vec3 mid = (m_vSceneMax + m_vSceneMin) / 2.0f; // Midpoint of Scene 

    glm::mat4 offset_mat = { // offset matrix to make object at origin
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        {-mid, 1}
    };

    glm::mat4 inverse_offset_mat = { // inverse of offset matrix to return object to original position
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        {mid, 1}
    };

    m_m4ViewMatrix[3][0] = 0.0f; 
    m_m4ViewMatrix[3][1] = 0.0f; 
    m_m4ViewMatrix[3][2] = 0.0f; 

    // float scale_factor = 1.0f; // optional scaling factor to determine how much space the scene should occupy when this function is called

    float radius = glm::length(m_vSceneMax - mid); // multiply scale_factor if desired

    float view_dist = radius / (glm::tan(glm::radians(25.f))); // viewing distance that is needed to look at all scene

    glm::vec3 view_vec = {0.0f, 0.0f, 1.0f}; 

    glm::vec3 look_from = mid + (view_vec * view_dist);

    // Camera still at origin
    sx = {1.0f, 0.0f, 0.0f};
    sy = {0.0f, 1.0f, 0.0f};
    sz = {0.0f, 0.0f, 1.0f};

    m_m4TempTransform[3][0] = -mid.x; //-glm::dot(sx, look_from);
    m_m4TempTransform[3][1] = -mid.y; //-glm::dot(sy, look_from); 
    m_m4TempTransform[3][2] = -(mid.z + view_dist); //-glm::dot(sz, look_from); 

    m_m4ViewMatrix = m_m4TempTransform * inverse_offset_mat * m_m4ViewMatrix * offset_mat; 
} 

void MyCamera::_getScreenXYZ(glm::vec3& sx, glm::vec3& sy, glm::vec3& sz)
{
    sx.x = m_m4ViewMatrix[0][0];
    sx.y = m_m4ViewMatrix[1][0];
    sx.z = m_m4ViewMatrix[2][0];
    sy.x = m_m4ViewMatrix[0][1];
    sy.y = m_m4ViewMatrix[1][1];
    sy.z = m_m4ViewMatrix[2][1];
    sz.x = m_m4ViewMatrix[0][2];
    sz.y = m_m4ViewMatrix[1][2];
    sz.z = m_m4ViewMatrix[2][2];
}

