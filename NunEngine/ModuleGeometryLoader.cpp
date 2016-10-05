#include "Application.h"
#include "ModuleGeometryLoader.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleGeometryLoader::ModuleGeometryLoader(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGeometryLoader::~ModuleGeometryLoader()
{
}

bool ModuleGeometryLoader::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

update_status ModuleGeometryLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometryLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometryLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGeometryLoader::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

void ModuleGeometryLoader::LoadGeometry(const char * path)
{
}




