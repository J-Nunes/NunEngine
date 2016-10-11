#include "Application.h"
#include "ModuleGeometryLoader.h"
#include "Mesh.h"
#include "glm\glm.hpp"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include <gl/GL.h>
#include "glut\glut.h"
#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

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

	for (int i = 0; i < CHECKERS_HEIGHT; i++)
	{
		for (int j = 0; j < CHECKERS_WIDTH; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	return true;
}

update_status ModuleGeometryLoader::PreUpdate(float dt)
{
	if (geometry_loaded == false)
	{
		LoadGeometry("Brutus.fbx");
		image_name = ilutGLLoadImage("Lenna.png");
		/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &image_name);
		glBindTexture(GL_TEXTURE_2D, image_name);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
			0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/

		geometry_loaded = true;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleGeometryLoader::Update(float dt)
{
	vector<Mesh*>::iterator it = meshes.begin();

	while (it != meshes.end())
	{
		App->renderer3D->DrawMesh((*it));
		it++;
	}

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
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh *mesh = new Mesh();
			aiMesh *ai_mesh = scene->mMeshes[i];
			LOG("Creating new mesh %s", ai_mesh->mName);

			// Copying vertices...
			mesh->num_vertices = ai_mesh->mNumVertices;
			mesh->vertices = new glm::vec3[mesh->num_vertices];
			memcpy(mesh->vertices, ai_mesh->mVertices, sizeof(glm::vec3) * mesh->num_vertices);
			LOG("  -> %d vertices", mesh->num_vertices);

			// Copying normals...
			mesh->num_normals = ai_mesh->mNumVertices;
			mesh->normals = new glm::vec3[mesh->num_normals];
			memcpy(mesh->normals, ai_mesh->mNormals, sizeof(glm::vec3) * mesh->num_normals);
			LOG("  -> %d normals", mesh->num_normals);

			uint UV_index = 0;
 			if (ai_mesh->HasTextureCoords(mesh->id_text))
 			{
 				mesh->num_text = ai_mesh->mNumVertices;
 				mesh->text = new glm::vec2[mesh->num_text];
				for (int i = 0; i < mesh->num_text; i++)
				{
					mesh->text[i].x = ai_mesh->mTextureCoords[mesh->id_text][i].x;
					mesh->text[i].y = ai_mesh->mTextureCoords[mesh->id_text][i].y;
				}
 				LOG("  -> %d texture coordinates", mesh->num_text);
 			}	

			if (ai_mesh->HasFaces())
			{
				mesh->num_indices = ai_mesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
				LOG("  -> %d indices", mesh->num_indices);
				for (uint j = 0; j < ai_mesh->mNumFaces; ++j)
				{
					if (ai_mesh->mFaces[j].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!");

					else
						memcpy(&mesh->indices[j * 3], ai_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
			App->renderer3D->LoadMesh(mesh);
			meshes.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);
}




