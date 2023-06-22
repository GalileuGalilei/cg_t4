#include "GameEvents.h"
#include "gl_canvas2d.h"
#include <string>

int OnMouseOverEvent::oldX = 0;
int OnMouseOverEvent::oldY = 0;

EventType OnMouseOverEvent::GetStaticType()
{
	return EventType::MouseOverEvent;
}

EventType OnRenderEvent::GetStaticType()
{
	return EventType::RenderEvent;
}

EventType OnClickEvent::GetStaticType()
{
	return EventType::ClickEvent;
}

EventType OnKeyEvent::GetStaticType()
{
	return EventType::KeyEvent;
}

clock_t OnUpdateEvent::oldClock = 0;

EventType OnUpdateEvent::GetStaticType()
{
	return EventType::UpdateEvent;
}

std::list<IRenderable*> IRenderable::renderList;
void IRenderable::RenderAll(BaseEvent* baseEvent)
{
	OnRenderEvent* args = (OnRenderEvent*)baseEvent;

	for (int i = GameLayer::LayersCount - 1; i >= 0; i--)
	{
		for (auto j : IRenderable::renderList)
		{
			if (i == j->GetLayer())
			{
				j->OnRender(args);
			}
		}
	}
}

std::list<IClickable*> IClickable::clickList;
void IClickable::ClickAll(BaseEvent* baseEvent)
{
	OnClickEvent* args = (OnClickEvent*)baseEvent;

	for (auto i : IClickable::clickList)
	{
		i->OnClick(args);
	}
}

void IClickable::MouseOverAll(BaseEvent* baseEvent)
{
	OnMouseOverEvent* args = (OnMouseOverEvent*)baseEvent;

	for (auto i : IClickable::clickList)
	{
		i->OnMouseOver(args);
	}
}

std::list<IKeyable*> IKeyable::keyList;
void IKeyable::KeyAll(BaseEvent* baseEvent)
{
	OnKeyEvent* args = (OnKeyEvent*)baseEvent;
	for (auto i : IKeyable::keyList)
	{
		i->OnKey(args);
	}
}

std::list<IUpdatable*> IUpdatable::updateList;
void IUpdatable::UpdateAll(BaseEvent* baseEvent)
{
	OnUpdateEvent* args = (OnUpdateEvent*)baseEvent;
	for (auto i : IUpdatable::updateList)
	{
		i->OnUpdate(args);

	}

	CV::color(1, 1, 1);
	std::string fps = "FPS: " + std::to_string((long)(1.0 / (double)args->deltaTime));
	CV::text(500, 10, fps.c_str());
	std::string objects = "Objects: " + std::to_string((long)updateList.size());
	CV::text(500, 30, objects.c_str());
}