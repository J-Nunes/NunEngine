#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

#include <string>


enum COMPONENT_TYPE
{
	TRANSFORMATION = 0,
	MESH,
	MATERIAL
};

class Component 
{
public:
	Component(COMPONENT_TYPE _type, const char* _name,GameObject* _game_object);
	virtual ~Component();

	virtual void CleanUp();

	const COMPONENT_TYPE	GetType()const;
	GameObject*				GetGameObject() const;
	void					Enable();
	void					Disable();
	const bool				IsActive() const;

public:
	std::string name;

private:
	COMPONENT_TYPE			type;
	bool					active = true;
	GameObject*				game_object = nullptr;

};

#endif 
