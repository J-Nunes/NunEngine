#ifndef __MODULE_GAME_OBJECT_H__
#define __MODULE_GAME_OBJECT_H__

#include "Module.h"
#include "Globals.h"

class GameObject;

class ModuleGameObject : public Module
{
public:
	ModuleGameObject(Application* app, bool start_enabled = true);
	~ModuleGameObject();

	bool				Init();
	update_status		Update(float dt);
	bool				CleanUp();

	GameObject*			CreateGameObject(const char* name, GameObject* parent = nullptr);

private:
	void				SetParent(GameObject* child, GameObject* parent);
	void				CleanAllGameObjects(GameObject* go_to_remove);

private:
	GameObject*			root = nullptr;
	uint				current_id = 0;
};
#endif // __MODULE_GEOMETRY_LOADER_H__
