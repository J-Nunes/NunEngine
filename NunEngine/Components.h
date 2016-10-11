#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

enum COMPONENT_TYPE
{
	TRANSFORMATION = 0,
	MESH,
	MATERIAL
};

class Component 
{
public:
	Component(COMPONENT_TYPE _type, GameObject* _game_object);
	virtual ~Component();

	const COMPONENT_TYPE	GetType()const;
	GameObject*				GetGameObject() const;
	void					Enable();
	void					Disable();
	const bool				IsActive() const;

private:
	COMPONENT_TYPE			type;
	bool					active = true;
	GameObject*				game_object = nullptr;

};

#endif 
