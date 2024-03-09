#ifndef __MY_GAMEOBJECT_H__
#define __MY_GAMEOBJECT_H__

#include "my_model.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <memory>

struct Transform2dComponent
{
	glm::vec2 translation{};  // (position offset)
	glm::vec2 scale{ 1.f, 1.f };
	float rotation;

	glm::mat2 mat2()
	{
		// note: glm matrix is column major
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);
		glm::mat2 rotMatrix{ {c, s}, {-s, c} };

		glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
		return rotMatrix * scaleMat;
	}
};

class MyGameObject
{
public:
	using id_t = unsigned int;

	static MyGameObject createGameObject(std::string name)
	{
		static id_t currentID = 0;
		return MyGameObject(currentID++, name);
	}

	MyGameObject(const MyGameObject&) = delete;
	MyGameObject& operator=(const MyGameObject&) = delete;
	MyGameObject(MyGameObject&&) = default;
	MyGameObject& operator=(MyGameObject&&) = default;

	std::string              getName() const { return m_sName; };
	id_t                     getID()   const { return m_iID; }
	std::shared_ptr<MyModel> model{};
	Transform2dComponent     transform2d{};
	float                    half_width; 
	float                    top_y; 
	float                    bottom_y; 

	float getX();
	float getY();
	void setX(float x);
	void setY(float y);
	void translate_X_by(float direction);
	void translate_Y_by(float direction);

protected:
	MyGameObject(id_t objID, std::string name) : m_iID{ objID }, m_sName{ name } {}
	id_t m_iID;
	std::string m_sName;
	
	glm::vec2 left_norm = {1.0f, 0.0f}; 
	glm::vec2 right_norm = {-1.0f, 0.0f}; 
	glm::vec2 top_norm = {0.0f, 1.0f}; 
	glm::vec2 bottom_norm = {0.0f, -1.0f}; 

	glm::vec2 direction{ 0.01f, 0.005f };
};


class MyPaddle : public MyGameObject
{

public:
	MyPaddle(); 

	void UpdateGameLogic(float offset_in_x, bool movingLeft, float paddle_speed);
};

class MyBall : public MyGameObject
{

public:
	MyBall(); 

	void UpdateGameLogic(MyPaddle *top_paddle, MyPaddle *bottom_paddle, float m_speed_multiplier);
};
#endif


