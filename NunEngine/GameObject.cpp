#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(const uint _id, const char * name) : tag(name), id(_id)
{
}

GameObject::~GameObject()
{
	std::vector<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it)->CleanUp();
		RELEASE(*it);
		it++;
	}
	components.clear();
}

bool GameObject::Update()
{
	return true;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::Enable()
{
	active = true;
}

void GameObject::Disable()
{
	active = false;
}

const bool GameObject::IsActive() const
{
	return active;
}
