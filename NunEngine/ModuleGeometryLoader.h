#ifndef __MODULE_GEOMETRY_LOADER_H__
#define __MODULE_GEOMETRY_LOADER_H__

#include "Module.h"
#include <vector>
#include "Globals.h"
#include "Glew\include\glew.h"

class ModuleGeometryLoader : public Module
{
public:
	ModuleGeometryLoader(Application* app, bool start_enabled = true);
	~ModuleGeometryLoader();

	bool				Init();
	update_status		PreUpdate(float dt);
	update_status		Update(float dt);
	update_status		PostUpdate(float dt);
	bool				CleanUp();

	void				LoadGeometry(const char* path);

public:
	uint image_name = 0;

private:
	bool geometry_loaded = false;
	std::vector<Mesh*> meshes;

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
};

#endif // !__MODULE_GEOMETRY_LOADER_H__
