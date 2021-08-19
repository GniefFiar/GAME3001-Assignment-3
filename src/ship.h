#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "Agent.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "PlayerAnimationState.h"
#include "Animation.h"
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
	Animation& getAnimation(const std::string& name);
	SpriteSheet* getSpriteSheet();
	// setters
	void setMaxSpeed(float newSpeed);
	void setHealth(int num);
	void setAnimationState(PlayerAnimationState new_state);
	void setSpriteSheet(SpriteSheet* sprite_sheet);
	void setAnimation(const Animation& animation);

private:
	void m_checkBounds();
	void m_reset();
	void m_buildAnimations();
	bool m_animationExists(const std::string& id);

	int m_health = 5;

	SpriteSheet* m_SpriteSheet;
	float m_maxSpeed;
	float m_turnRate;
	PlayerAnimationState m_currentAnimationState;
	std::unordered_map<std::string, Animation> m_pAnimations;
};


#endif /* defined (__SHIP__) */

