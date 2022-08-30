#pragma once
#include <SFML/System.hpp>
#include <math.h>

#define PI 3.14159265

struct Direction
{
public:
	Direction()
		: m_angle(0.0f)
		, m_rotation_speed(250.0f)
	{}

	Direction(float angle, float rotation_speed = 250.0f)
		: m_angle(angle)
		, m_rotation_speed(rotation_speed)
	{}

	float getAngle() const
	{
		return m_angle;
	}
	
	sf::Vector2f getVector()
	{
		updateVec();
		return m_vec;
	}
	
	void setAngle(float angle)
	{
		m_angle = angle;
	}
	
	float update(float dt, float angular_velocity)
	{
		m_angle = fmod(m_angle + angular_velocity * m_rotation_speed * dt + 360, 360);
		return m_angle;
	}
		
private:

	void updateVec()
	{
		m_vec.x = cos(m_angle * PI / 180.0);
		m_vec.y = sin(m_angle * PI / 180.0);
	}
	sf::Vector2f m_vec;
	float m_angle;
	float m_rotation_speed;
};