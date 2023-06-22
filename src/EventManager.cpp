#include "EventManager.h"
#include <type_traits>

EventType BaseEvent::GetStaticType()
{
	return EventType::None;
}

using EventFunc = void(*)(BaseEvent*);
using EventList = std::list<EventFunc>;
using EventMap = EventList[EventType::Count];

EventMap EventManager::eventMap;
EventManager* EventManager::instance = NULL;

EventManager* EventManager::Instance()
{
	if (instance == NULL)
	{
		instance = new EventManager();
	}

	return instance;
}