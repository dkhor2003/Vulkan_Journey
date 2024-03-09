#include "my_game_object.h"

float MyGameObject::getX()
{
    return transform2d.translation.x; 
}

float MyGameObject::getY()
{
    return transform2d.translation.y;
}

void MyGameObject::setX(float x)
{
    transform2d.translation.x = x; 
}

void MyGameObject::setY(float y)
{
    transform2d.translation.y = y; 
}

void MyGameObject::translate_X_by(float direction)
{
    transform2d.translation.x += direction; 
}

void MyGameObject::translate_Y_by(float direction)
{
    transform2d.translation.y += direction;
}

void MyPaddle::UpdateGameLogic(float offset_in_x, bool movingLeft, float paddle_speed)
{
    if (movingLeft)
    {
        if (getX() - offset_in_x <= -1.0f)
        {
            setX(-1.0f + offset_in_x); 
        }
        else
        {
            translate_X_by(-paddle_speed); 
        }
    }
    else
    {
        if (getX() + offset_in_x >= 1.0f)
        {
            setX(1.0f - offset_in_x);  
        }
        else
        {
            translate_X_by(paddle_speed);
        }
    }
} 

void MyBall::UpdateGameLogic(MyPaddle *top_paddle, MyPaddle *bottom_paddle, float m_speed_multiplier){

    transform2d.rotation = glm::mod(transform2d.rotation + 0.5f, glm::two_pi<float>());

    // If ball hit bottom paddle top side
    if (getY() >= (bottom_paddle->top_y - half_width) && 
        getY() <= (bottom_paddle->top_y) && 
        getX() >= bottom_paddle->getX() - bottom_paddle->half_width && 
        getX() <= bottom_paddle->getX() + bottom_paddle->half_width)  
    {
        direction = direction - 2 * glm::dot(bottom_norm , direction) * bottom_norm; 
    }

    // If ball hit top paddle bottom side
    if (getY() <= (top_paddle->bottom_y + half_width) && 
        getY() >= (top_paddle->bottom_y) && 
        getX() >= top_paddle->getX() - top_paddle->half_width && 
        getX() <= top_paddle->getX() + top_paddle->half_width)
    {
        direction = direction - 2 * glm::dot(top_norm , direction) * top_norm; 
    }

    // If ball hit right wall
    if (getX() >= 1.0f - half_width)
    {
        direction = direction - 2 * glm::dot(right_norm , direction) * right_norm; 
    }

    // If ball hit left wall
    if (getX() <= -1.0f + half_width)
    { 
        direction = direction - 2 * glm::dot(left_norm , direction) * left_norm; 
    }

    // Move ball correspondingly
    translate_X_by(direction.x);
    translate_Y_by(direction.y * m_speed_multiplier); 
}