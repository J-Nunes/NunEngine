#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Mesh.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"
#include "glm\gtc\type_ptr.hpp"


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum gl_enum = glewInit();

	if (GLEW_OK != gl_enum)
	{
		LOG("Glew failed");
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT, 20.0f);

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	ImGui::Render();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::CubePaintDirectMode()
{
	glBegin(GL_TRIANGLES);

	glColor3f(255.f, 0.f, 0.f);

	glVertex3f(0.f, 0.f, 0.f);//A
	glVertex3f(5.f, 0.f, 0.f);//B
	glVertex3f(5.f, 5.f, 0.f);//C

	glVertex3f(5.f, 5.f, 0.f);//C
	glVertex3f(0.f, 5.f, 0.f);//D
	glVertex3f(0.f, 0.f, 0.f);//A

	glColor3f(0.f, 255.f, 0.f);

	glVertex3f(5.f, 0.f, 0.f);//B
	glVertex3f(5.f, 0.f, -5.f);//E
	glVertex3f(5.f, 5.f, -5.f);//F

	glVertex3f(5.f, 5.f, -5.f);//F
	glVertex3f(5.f, 5.f, 0.f);//C
	glVertex3f(5.f, 0.f, 0.f);//B

	glColor3f(0.f, 0.f, 255.f);

	glVertex3f(5.f, 0.f, -5.f);//E
	glVertex3f(0.f, 0.f, -5.f);//G
	glVertex3f(5.f, 5.f, -5.f);//F

	glVertex3f(5.f, 5.f, -5.f);//F
	glVertex3f(0.f, 0.f, -5.f);//G
	glVertex3f(0.f, 5.f, -5.f);//H

	glColor3f(125.f, 125.f, 0.f);

	glVertex3f(0.f, 5.f, -5.f);//F
	glVertex3f(0.f, 0.f, -5.f);//E
	glVertex3f(0.f, 0.f, 0.f);//B

	glVertex3f(0.f, 0.f, 0.f);//B
	glVertex3f(0.f, 5.f, 0.f);//C
	glVertex3f(0.f, 5.f, -5.f);//F

	glColor3f(0.f, 125.f, 125.f);

	glVertex3f(0.f, 5.f, 0.f);//D
	glVertex3f(5.f, 5.f, 0.f);//C
	glVertex3f(5.f, 5.f, -5.f);//F

	glVertex3f(0.f, 5.f, 0.f);//D
	glVertex3f(5.f, 5.f, -5.f);//F
	glVertex3f(0.f, 5.f, -5.f);//H

	glColor3f(125.f, 0.f, 125.f);

	glVertex3f(0.f, 0.f, 0.f);//D
	glVertex3f(5.f, 0.f, -5.f);//F
	glVertex3f(5.f, 0.f, 0.f);//C

	glVertex3f(0.f, 0.f, 0.f);//D
	glVertex3f(0.f, 0.f, -5.f);//H
	glVertex3f(5.f, 0.f, -5.f);//F
	glEnd();
}

uint ModuleRenderer3D::CubeVertexArray()
{
	vector<glm::vec3> vertices;

	//FRONT
	vertices.push_back(glm::vec3(0.f, 0.f, 0.f));//A
	vertices.push_back(glm::vec3(5.f, 0.f, 0.f));//B
	vertices.push_back(glm::vec3(5.f, 5.f, 0.f));//C

	vertices.push_back(glm::vec3(5.f, 5.f, 0.f));//C
	vertices.push_back(glm::vec3(0.f, 5.f, 0.f));//D
	vertices.push_back(glm::vec3(0.f, 0.f, 0.f));//A

	//RIGHT SIDE									   
	vertices.push_back(glm::vec3(5.f, 0.f, 0.f));//B
	vertices.push_back(glm::vec3(5.f, 0.f, -5.f));//E
	vertices.push_back(glm::vec3(5.f, 5.f, -5.f));//F

	vertices.push_back(glm::vec3(5.f, 5.f, -5.f));//F
	vertices.push_back(glm::vec3(5.f, 5.f, 0.f));//C
	vertices.push_back(glm::vec3(5.f, 0.f, 0.f));//B

	//BACK									   
	vertices.push_back(glm::vec3(5.f, 0.f, -5.f));//E
	vertices.push_back(glm::vec3(0.f, 0.f, -5.f));//G
	vertices.push_back(glm::vec3(5.f, 5.f, -5.f));//F

	vertices.push_back(glm::vec3(5.f, 5.f, -5.f));//F
	vertices.push_back(glm::vec3(0.f, 0.f, -5.f));//G
	vertices.push_back(glm::vec3(0.f, 5.f, -5.f));//H

	//LEFT SIDE										
	vertices.push_back(glm::vec3(0.f, 5.f, -5.f));//H
	vertices.push_back(glm::vec3(0.f, 0.f, -5.f));//G
	vertices.push_back(glm::vec3(0.f, 0.f, 0.f));//A

	vertices.push_back(glm::vec3(0.f, 0.f, 0.f));//A
	vertices.push_back(glm::vec3(0.f, 5.f, 0.f));//D
	vertices.push_back(glm::vec3(0.f, 5.f, -5.f));//H

	//TOP										
	vertices.push_back(glm::vec3(0.f, 5.f, 0.f));//D
	vertices.push_back(glm::vec3(5.f, 5.f, 0.f));//C
	vertices.push_back(glm::vec3(5.f, 5.f, -5.f));//F

	vertices.push_back(glm::vec3(0.f, 5.f, 0.f));//D
	vertices.push_back(glm::vec3(5.f, 5.f, -5.f));//F
	vertices.push_back(glm::vec3(0.f, 5.f, -5.f));//H

	//BOTTOM										
	vertices.push_back(glm::vec3(0.f, 0.f, 0.f));//A
	vertices.push_back(glm::vec3(5.f, 0.f, -5.f));//E
	vertices.push_back(glm::vec3(5.f, 0.f, 0.f));//B

	vertices.push_back(glm::vec3(0.f, 0.f, 0.f));//A
	vertices.push_back(glm::vec3(0.f, 0.f, -5.f));//G
	vertices.push_back(glm::vec3(5.f, 0.f, -5.f));//E

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);

	return vertices.size();
}

void ModuleRenderer3D::DrawCubeVertexArray(uint size)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, size * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

uint ModuleRenderer3D::CubeIndices()
{
	vector<glm::vec3> vertex;

	vertex.push_back(glm::vec3(5.f, 5.f, 0.f));//C 0
	vertex.push_back(glm::vec3(0.f, 5.f, 0.f));//D 1
	vertex.push_back(glm::vec3(0.f, 0.f, 0.f));//A 2
	vertex.push_back(glm::vec3(5.f, 0.f, 0.f));//B 3
	vertex.push_back(glm::vec3(5.f, 0.f, -5.f));//E 4
	vertex.push_back(glm::vec3(5.f, 5.f, -5.f));//F 5
	vertex.push_back(glm::vec3(0.f, 5.f, -5.f));//H 6
	vertex.push_back(glm::vec3(0.f, 0.f, -5.f));//G 7

	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex.size() * 3, vertex.data(), GL_STATIC_DRAW);

	vector<uint> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(0);

	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(1);
	indices.push_back(0);

	indices.push_back(1);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(7);
	indices.push_back(4);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(7);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(4);

	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), indices.data(), GL_STATIC_DRAW);

	return indices.size();
}

void ModuleRenderer3D::DrawCubeIndices(uint size)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::OnResize(int width, int height, float fovy)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float coty = 1.0f / tan(60.0f * (float)M_PI / 360.0f);

	ProjectionMatrix[0][0] = coty / ((float)width / (float)height);
	ProjectionMatrix[1][1] = coty;
	ProjectionMatrix[2][2] = (0.125f + 512.0f) / (0.125f - 512.0f);
	ProjectionMatrix[2][3] = -1.0f;
	ProjectionMatrix[3][2] = 2.0f * 0.125f * 512.0f / (0.125f - 512.0f);
	ProjectionMatrix[3][3] = 0.0f;

	//ProjectionMatrix = perspective(fovy, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(glm::value_ptr(ProjectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ModuleRenderer3D::LoadMesh(Mesh* mesh)
{
	// Vertices
	glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
	if (mesh->id_vertices == 0)
		return false;

	else
	{
		const uint num_vertices = mesh->num_vertices * 3;
		float* vertices = new float[num_vertices];
		for (uint i = 0, j = 0; i < mesh->num_vertices; i++, j++)
		{
			vertices[j] = mesh->vertices[i].x;
			vertices[++j] = mesh->vertices[i].y;
			vertices[++j] = mesh->vertices[i].z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);
	}

	// Normals
	glGenBuffers(1, (GLuint*) &(mesh->id_normals));
	if (mesh->id_normals == 0)
		return false;

	else
	{
		const uint num_normals = mesh->num_normals * 3;
		float* normals = new float[num_normals];
		for (uint i = 0, j = 0; i < mesh->num_normals; i++, j++)
		{
			normals[j] = mesh->normals[i].x;
			normals[++j] = mesh->normals[i].y;
			normals[++j] = mesh->normals[i].z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_normals, normals, GL_STATIC_DRAW);
	}

	// Texture coordinates
	glGenBuffers(1, (GLuint*) &(mesh->id_text));
	if (mesh->id_text == 0)
			return false;

	else
		{
			const uint num_text = mesh->num_text * 2;
			float* textures = new float[num_text];
			for (uint i = 0, j = 0; i < mesh->num_text; i++, j++)
			{
				textures[j] = mesh->text[i].x;
				textures[++j] = mesh->text[i].y;
			}

			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_text);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_text, textures, GL_STATIC_DRAW);
		}

	// Indices
	glGenBuffers(1, (GLuint*) &(mesh->id_indices));
	if (mesh->id_indices == 0)
		return false;

	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*mesh->num_indices, mesh->indices, GL_STATIC_DRAW);
	}

	return true;
}

void ModuleRenderer3D::DrawMesh(const Mesh * mesh)
{
	glColor4f(1, 1, 1, 1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_text);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	glBindTexture(GL_TEXTURE_2D, App->geometry_loader->image_name);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
