#ifndef __MODULE_GEOMETRY_LOADER_H__
#define __MODULE_GEOMETRY_LOADER_H__

#include "Module.h"
#include <vector>

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

private:
	bool geometry_loaded = false;
	std::vector<Mesh*> meshes;
};

#endif // !__MODULE_GEOMETRY_LOADER_H__
