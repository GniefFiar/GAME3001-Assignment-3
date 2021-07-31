#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "Agent.h"
#include "TextureManager.h"
#include <glm/vec4.hpp>

class Ship final : public Agent
{
public:
	Ship();
	~Ship();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();

	// getters
	float getMaxSpeed() const;
	int getHealth();
	// setters
	void setMaxSpeed(float newSpeed);
	void setHealth(int num);

private:
	void m_checkBounds();
	void m_reset();

	int m_health = 5;

	float m_maxSpeed;
	float m_turnRate;
};


#endif /* defined (__SHIP__) */
