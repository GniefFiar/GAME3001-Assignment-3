#include "ship.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include "Sprite.h"
#include "Animation.h"
#include "PlayerAnimationState.h"

Ship::Ship() : m_maxSpeed(2.0f)
{
	TextureManager::Instance().loadSpriteSheet("../Assets/sprites/GNN.txt", "../Assets/sprites/Galactic Nova Nucleus.png", "GNN");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("GNN"));

	// set frame width
	GameObject::setWidth(31);

	// set frame height
	GameObject::setHeight(27);

	GameObject::getTransform()->position = glm::vec2(400.0f, 300.0f);
	GameObject::getRigidBody()->velocity = glm::vec2(2.0f, 0.0f);
	GameObject::getRigidBody()->acceleration = glm::vec2(2.0f, 0.0f);
	GameObject::getRigidBody()->isColliding = false;
	GameObject::setType(AGENT);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));
	m_buildAnimations();
}


Ship::~Ship()
= default;

void Ship::draw()
{
	// alias for x and y
	const auto x = GameObject::getTransform()->position.x;
	const auto y = GameObject::getTransform()->position.y;

	// draw the player according to animation state
	switch (m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance().playAnimation("GNN", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance().playAnimation("GNN", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance().playAnimation("GNN", getAnimation("run"),
			x, y, 0.25f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance().playAnimation("GNN", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}

	// draw LOS
	Util::DrawLine(GameObject::getTransform()->position, GameObject::getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());
}


void Ship::update()
{
	move();
	m_checkBounds();
}

void Ship::clean()
{
}

void Ship::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Ship::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Ship::moveForward()
{
	GameObject::getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Ship::moveBack()
{
	GameObject::getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Ship::move()
{
	GameObject::getTransform()->position += GameObject::getRigidBody()->velocity;
	GameObject::getRigidBody()->velocity *= 1.0f;
}

float Ship::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Ship::m_checkBounds()
{

	if (GameObject::getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		moveBack();
	}

	if (GameObject::getTransform()->position.x < 0)
	{
		moveForward();
	}

	if (GameObject::getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		moveBack();
	}

	if (GameObject::getTransform()->position.y < 0)
	{
		moveForward();
	}

}

void Ship::m_reset()
{
	GameObject::getRigidBody()->isColliding = false;
	const int halfWidth = GameObject::getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - GameObject::getWidth()) + halfWidth + 1;
	const auto yComponent = -GameObject::getHeight();
	GameObject::getTransform()->position = glm::vec2(xComponent, yComponent);
}

bool Ship::m_animationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}

Animation& Ship::getAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

void Ship::setHealth(int num)
{
	m_health += num;
}

void Ship::setSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_SpriteSheet = sprite_sheet;
}

void Ship::setAnimation(const Animation& animation)
{
	if (!m_animationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

void Ship::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

int Ship::getHealth()
{
	return m_health;
}

SpriteSheet* Ship::getSpriteSheet()
{
	return m_SpriteSheet;
}

void Ship::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-3"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("GNN-idle-3"));

	setAnimation(runAnimation);
}