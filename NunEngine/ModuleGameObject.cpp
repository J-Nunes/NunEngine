#include "Application.h"
#include "ModuleGameObject.h"
#include "GameObject.h"

ModuleGameObject::ModuleGameObject(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Init()
{
	return true;
}

update_status ModuleGameObject::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGameObject::CleanUp()
{
	return true;
}

GameObject * ModuleGameObject::CreateGameObject(const char * name, GameObject * parent)
{
	GameObject* ret = new GameObject(current_id++, name);

	SetParent(ret, parent);

	return ret;
}

bool ModuleGameObject::SetParent(GameObject * child, GameObject * parent)
{
	bool ret = false;

	if (child && child != root)
	{
		//Remove child from its actual parent vector of children
		if (child->parent)
		{
			GameObject* actual_parent = child->parent;
			for (vector<GameObject*>::iterator item = actual_parent->children.begin(); item != actual_parent->children.end(); item++)
			{
				if ((*item)->id == child->id)
					actual_parent->children.erase(item);
			}
		}
		//Set child's parent and include child on the new parent's vector of children
		if (parent)
		{
			child->parent = parent;
			parent->children.push_back(child);
		}

		else
		{
			child->parent = root;
			root->children.push_back(child);
		}

		ret = true;		
	}
	return ret;
}
