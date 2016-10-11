#include "GameObject.h"

GameObject::GameObject(const uint _id, const char * name) : tag(name), id(_id)
{
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	return true;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}