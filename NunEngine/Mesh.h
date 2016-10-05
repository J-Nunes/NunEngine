#ifndef __MESH_H__
#define __MESH_H__

#include "Globals.h"

struct Mesh
{
	uint id_indices = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; 
	uint num_vertices = 0;
	glm::vec3* vertices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	glm::vec3* normals = nullptr;
};

#endif __MESH_H__