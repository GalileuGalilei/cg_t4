#ifndef GAME_MANAGER
#define GAME_MANAGER

#include "gl_canvas2d.h"
#include "GameEvents.h"
#include <chrono>

class GameManager : public IUpdatable
{
private:

	GameManager();
	~GameManager();

	static GameManager* instance;
	static void ClockEventListener(BaseEvent* event);

public:
	static GameManager* Instance();
	static float deltaTime;
	static double time;
	Vector2 screenSize;

	void OnUpdate(OnUpdateEvent* args) override;
	void SetScreen(Vector2 screenSize);
};


#endif // !GAME_MANAGER