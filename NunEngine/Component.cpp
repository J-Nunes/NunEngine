#include "Component.h"
#include "GameObject.h"

Component::Component(COMPONENT_TYPE _type, const char* _name, GameObject* _game_object) : type(_type), name(_name), game_object(_game_object)
{
}

Component::~Component()
{
}

void Component::CleanUp()
{
}

const COMPONENT_TYPE Component::GetType() const
{
	return type;
}

GameObject* Component::GetGameObject() const
{
	return game_object;
}

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = false;
}

const bool Component::IsActive() const
{
	return active;
}
