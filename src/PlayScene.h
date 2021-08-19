#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "ship.h"
#include "PathNode.h"
#include "Target.h"
#include "DecisionTree.h"
#include "Plane.h"
#include "Player.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	//void moveStarShip() const;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	// Game Objects
	Target* m_pTarget;
	Ship* m_pShip;
	Obstacle* m_pObstacle1;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
	Obstacle* m_pObstacle4;
	Obstacle* m_pObstacle5;
	Obstacle* m_pObstacle6;
	Player* m_pDragoon;

	// Other
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	bool m_CheckAgentLOS(Agent* agent, DisplayObject* object);
	void m_CheckPathNodeLOS();
	PathNode* m_findClosestPathNode(Agent* agent);
	void m_toggleGrid(bool state);

	// Decision Tree
	DecisionTree* decisionTree;
};

#endif /* defined (__PLAY_SCENE__) */