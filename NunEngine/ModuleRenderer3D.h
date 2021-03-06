#pragma once
#include "Module.h"
#include "Globals.h"
#include "glm\glm.hpp"
#include "Light.h"

#define MAX_LIGHTS 8

class Mesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CubePaintDirectMode();
	uint CubeVertexArray();
	void DrawCubeVertexArray(uint size);
	uint CubeIndices();
	void DrawCubeIndices(uint size);

	void OnResize(int width, int height, float fovy);
	bool LoadMesh(Mesh* mesh);
	void DrawMesh(const Mesh* mesh);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	glm::mat3x3 NormalMatrix;
	glm::mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

private:
	uint indices_size;
	uint my_indices = 0;
	uint vertex_size;
	uint my_id = 0;
};