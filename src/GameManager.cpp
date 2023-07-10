#include "GameManager.h"
#include "Mesh.h"
#include <chrono>


GameManager* GameManager::instance = nullptr;

GameManager* GameManager::Instance()
{
	if (instance == nullptr)
	{
		instance = new GameManager();
	}

	return instance;
}

GameManager::GameManager()
{
	srand(std::time(NULL));

	EventManager::Instance()->AddListener<OnUpdateEvent>(ClockEventListener);
	EventManager::Instance()->AddListener<OnUpdateEvent>(IUpdatable::UpdateAll);
	EventManager::Instance()->AddListener<OnRenderEvent>(IRenderable::RenderAll);
	EventManager::Instance()->AddListener<OnClickEvent>(IClickable::ClickAll);
	EventManager::Instance()->AddListener<OnMouseOverEvent>(IClickable::MouseOverAll);
	EventManager::Instance()->AddListener<OnKeyEvent>(IKeyable::KeyAll);
}

GameManager::~GameManager()
{
	EventManager::Instance()->RemoveListener<OnUpdateEvent>(ClockEventListener);
	EventManager::Instance()->RemoveListener<OnUpdateEvent>(IUpdatable::UpdateAll);
	EventManager::Instance()->RemoveListener<OnRenderEvent>(IRenderable::RenderAll);
	EventManager::Instance()->RemoveListener<OnClickEvent>(IClickable::ClickAll);
	EventManager::Instance()->RemoveListener<OnMouseOverEvent>(IClickable::MouseOverAll);
	EventManager::Instance()->RemoveListener<OnKeyEvent>(IKeyable::KeyAll);
}

float GameManager::deltaTime = 0;
double GameManager::time = 0;

void GameManager::ClockEventListener(BaseEvent* event)
{
	OnUpdateEvent* args = (OnUpdateEvent*)event;
	deltaTime = args->deltaTime;
	time += deltaTime;
}

void GameManager::OnUpdate(OnUpdateEvent* args)
{
	Mesh::resetZBuffer();
}

void GameManager::SetScreen(Vector2 screenSize)
{
	screenSize = screenSize;
	Mesh::SetScreen(screenSize.x, screenSize.y);
	Mesh::SetLightSource(Vector3(200, 400, 10));
}
