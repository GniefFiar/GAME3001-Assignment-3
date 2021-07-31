#include "Plane.h"
#include "TextureManager.h"

Plane::Plane()
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/Dragoon.txt",
		"../Assets/sprites/Dragoon.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(33);

	// set frame height
	setHeight(36);

	getTransform()->position = glm::vec2(400.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

	m_buildAnimations();
}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance().playAnimation(
		"spritesheet", getAnimation("Dragoon"),
		x, y, 0.5f, 0, 255, true);
}

void Plane::update()
{
}

void Plane::clean()
{
}

void Plane::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "Dragoon";
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("Dragoon"));

	setAnimation(planeAnimation);
}