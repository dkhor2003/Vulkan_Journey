#include "my_keyboard_controller.h"

// std
#include <limits>
#include <iostream>

void MyKeyboardController::moveInPlaneXZ(GLFWwindow* window, float dt, MyGameObject& gameObject)
{
     // Camera rotation
     glm::vec3 rotate{ 0 };
     if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) 
     {
        rotate.y -= 1.f; // Y is up
        std::cout << "Looking to the right" << std::endl;
     }
     if (glfwGetKey(window, keys.lookLeft)  == GLFW_PRESS) 
     {
        rotate.y += 1.f;
        std::cout << "Looking to the left" << std::endl;
     }
     if (glfwGetKey(window, keys.lookUp)    == GLFW_PRESS) 
     {
        rotate.x += 1.f;
        std::cout << "Looking up" << std::endl;
     }
     if (glfwGetKey(window, keys.lookDown)  == GLFW_PRESS) 
     {
        rotate.x -= 1.f;
        std::cout << "Looking down" << std::endl;
     }
     if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
     {
         gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
     }

     // limit pitch values between about +/- 85ish degrees
     gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
     gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

     float yaw = gameObject.transform.rotation.y;
     const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
     const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
     const glm::vec3 upDir{ 0.f, 1.f, 0.f };

     glm::vec3 moveDir{ 0.f };
     if (glfwGetKey(window, keys.moveForward)  == GLFW_PRESS) 
     {
        moveDir -= forwardDir;
        std::cout << "Moving camera forward" << std::endl; 
     }
     if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) 
     {
        moveDir += forwardDir;
        std::cout << "Moving camera backward" << std::endl; 
     }
     if (glfwGetKey(window, keys.moveRight)    == GLFW_PRESS) 
     {
        moveDir += rightDir;
        std::cout << "Moving camera to the right" << std::endl; 
     }
     if (glfwGetKey(window, keys.moveLeft)     == GLFW_PRESS) 
     {
        moveDir -= rightDir;
        std::cout << "Moving camera to the left" << std::endl; 
     }
     if (glfwGetKey(window, keys.moveUp)       == GLFW_PRESS) 
     {
        moveDir += upDir;
        std::cout << "Moving camera upwards" << std::endl; 
     }
     if (glfwGetKey(window, keys.moveDown)     == GLFW_PRESS) 
     {
        moveDir -= upDir;
        std::cout << "Moving camera downwards" << std::endl; 
     }
     if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
     {
         gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
     }
}

