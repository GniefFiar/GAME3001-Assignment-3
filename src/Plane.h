#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"
#include "EventManager.h"

class Plane final : public Sprite
{
public:
	Plane();
	~Plane();
	int m_speed = 2;

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	void m_buildAnimations();
};

#endif /* defined (__PLANE__) */
