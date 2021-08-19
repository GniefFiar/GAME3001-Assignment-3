#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	
	m_CheckAgentLOS(m_pShip, m_pTarget);
	m_CheckPathNodeLOS();

	if (CollisionManager::AABBCheck(m_pDragoon, m_pObstacle1) || CollisionManager::AABBCheck(m_pDragoon, m_pObstacle2) ||
		CollisionManager::AABBCheck(m_pDragoon, m_pObstacle3) || CollisionManager::AABBCheck(m_pDragoon, m_pObstacle4) ||
		CollisionManager::AABBCheck(m_pDragoon, m_pObstacle5) || CollisionManager::AABBCheck(m_pDragoon, m_pObstacle6))
	{
		m_pDragoon->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);

	}

	// Set three conditions of decision tree here.
	//decisionTree->getLOSNode()->setLOS(decisionTree->getAgent()->hasLOS()); // Or use m_pShip if you want to be lazy/wrong.
	//decisionTree->getRadiusNode()->setIsWithinRadius(
	//	Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 400.0f);
	//decisionTree->getCloseCombatNode()->setIsWithinCombatRange(
	//	Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 60.0f);
}

void PlayScene::clean()
{
	removeAllChildren();
	// Clean up DecisionTree because it's not a DisplayObject.
	decisionTree->clean();
	delete decisionTree; 
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		m_pDragoon->getTransform()->position -= glm::vec2(0.0f, 2.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pDragoon->setAnimationState(PLAYER_RUN_LEFT);
		m_pDragoon->getTransform()->position -= glm::vec2(2.0f, 0.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		m_pDragoon->getTransform()->position += glm::vec2(0.0f, 2.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pDragoon->setAnimationState(PLAYER_RUN_RIGHT);
		m_pDragoon->getTransform()->position += glm::vec2(2.0f, 0.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
	{
		m_pShip->setHealth(-1);
		if (m_pShip->getHealth() == 0)
		{
			TextureManager::Instance().removeTexture("GNN");
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/EnemyDeath.wav", "EnemyDeath", SOUND_SFX);
			SoundManager::Instance().playSound("EnemyDeath", 0);
			SoundManager::Instance().setSoundVolume(50);
		}

		else
		{
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/EnemyHit.wav", "EnemyHit", SOUND_SFX);
			SoundManager::Instance().playSound("EnemyHit", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_A))
	{
		m_pDragoon->setAnimationState(PLAYER_IDLE_LEFT);
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_D))
	{
		m_pDragoon->setAnimationState(PLAYER_IDLE_RIGHT);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	SoundManager::Instance().allocateChannels(16);
	SoundManager::Instance().load("../Assets/audio/Zook.ogg", "Zook", SOUND_MUSIC);
	SoundManager::Instance().playMusic("Zook", -1);
	SoundManager::Instance().setMusicVolume(50);

	m_buildGrid();

	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->GameObject::getTransform()->position = glm::vec2(150.f, 300.f);
	addChild(m_pShip, 3);

	// add the Obstacle to the scene as a start point
	m_pObstacle1 = new Obstacle();
	m_pObstacle1->getTransform()->position = glm::vec2(100.f, 100.f);
	addChild(m_pObstacle1);

	// add the Obstacle to the scene as a start point
	m_pObstacle2 = new Obstacle();
	m_pObstacle2->getTransform()->position = glm::vec2(100.f, 280.f);
	addChild(m_pObstacle2);

	// add the Obstacle to the scene as a start point
	m_pObstacle3 = new Obstacle();
	m_pObstacle3->getTransform()->position = glm::vec2(380.f, 480.f);
	addChild(m_pObstacle3);

	m_pObstacle4 = new Obstacle();
	m_pObstacle4->getTransform()->position = glm::vec2(580.f, 180.f);
	addChild(m_pObstacle4);

	m_pObstacle5 = new Obstacle();
	m_pObstacle5->getTransform()->position = glm::vec2(350.f, 300.f);
	addChild(m_pObstacle5);

	m_pObstacle6 = new Obstacle();
	m_pObstacle6->getTransform()->position = glm::vec2(300.f, 150.f);
	addChild(m_pObstacle6);

	// added the target to the scene a goal
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(600.f, 300.f);
	addChild(m_pTarget);

	m_pDragoon = new Player();
	m_pDragoon->getTransform()->position = glm::vec2(500.f, 300.f);
	addChild(m_pDragoon);

	// Create test tree
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pShip);
	decisionTree->Display(); // Optional.

	/*std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;*/

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// TODO:
	// Toggle Visibility for the StarShip and the Target
		
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - M2021 - Lab 9", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool gridVisible = true;
	if (ImGui::Checkbox("Toggle Grid", &gridVisible))
	{
		m_toggleGrid(gridVisible);
	}

	ImGui::Separator();

	// allow ship rotation
	static int angle;
	if (ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pShip->setCurrentHeading(angle);

		/*std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pShip->GameObject::getTransform()->position.x, m_pShip->GameObject::getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pShip->GameObject::getTransform()->position.x = shipPosition[0];
		m_pShip->GameObject::getTransform()->position.y = shipPosition[1];

		/*std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
	}

	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];

		/*std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
	}

	ImGui::Separator();
	
	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);
			addChild(path_node);
			m_pGrid.push_back(path_node);
		}
	}
}

void PlayScene::m_toggleGrid(bool state)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

bool PlayScene::m_CheckAgentLOS(Agent* agent, DisplayObject* target_object)
{
	bool hasLOS = false;
	agent->setHasLOS(hasLOS);
	// if ship to target distance is less than or equal to LOS Distance
	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target_object );
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			if ((object->getType() != agent->getType()) && (object->getType() != target_object->getType()))
			{
				// check if object is farther than than the target
				auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, object);
				if (AgentToObjectDistance <= AgentToTargetDistance)
					contactList.push_back(object);
			}
		}
		contactList.push_back(target_object);
		auto agentTarget = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		auto hasLOS = CollisionManager::LOSCheck(agent, agentTarget, contactList, target_object);
		agent->setHasLOS(hasLOS);
	}
	return hasLOS;
}

void PlayScene::m_CheckPathNodeLOS()
{
	for (auto path_node : m_pGrid)
	{
		auto targetDirection = m_pShip->GameObject::getTransform()->position - path_node->getTransform()->position;
		auto normalizeDirection = Util::normalize(targetDirection);
		path_node->setCurrentDirection(normalizeDirection);
		m_CheckAgentLOS(path_node, m_pShip);
	}
}

PathNode* PlayScene::m_findClosestPathNode(Agent* agent)
{
	auto min = FLT_MAX;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pGrid)
	{
		const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
		if (distance < min ) // && path_node->hasLOS()
		{
			min = distance;
			closestPathNode = path_node;
		}
	}
	return closestPathNode;
}