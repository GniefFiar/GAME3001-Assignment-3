#pragma once
#ifndef __UFO__
#define __UFO__

#include "Agent.h"
#include "TextureManager.h"
#include <glm/vec4.hpp>

class UFO final : public Agent
{
public:
	UFO();
	~UFO();

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
	// setters
	void setMaxSpeed(float newSpeed);

private:
	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;
};


#endif