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
	if (root)
	{
		CleanAllGameObjects(root);
		RELEASE(root);
	}

	return true;
}

GameObject * ModuleGameObject::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* ret = new GameObject(current_id++, name);

	SetParent(ret, parent);

	return ret;
}

void ModuleGameObject::SetParent(GameObject* child, GameObject* parent)
{
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
	}
}

void ModuleGameObject::CleanAllGameObjects(GameObject* go_to_remove)
{
	if (go_to_remove->children.size() > 0)
	{
		for (vector<GameObject*>::iterator item = go_to_remove->children.begin(); item != go_to_remove->children.end(); item++)
			CleanAllGameObjects((*item));

		for (vector<GameObject*>::iterator item = go_to_remove->children.begin(); item != go_to_remove->children.end(); item++)
			RELEASE((*item));

		go_to_remove->children.clear();
	}
}
