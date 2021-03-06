#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = glm::vec3(1.0f, 0.0f, 0.0f);
	Y = glm::vec3(0.0f, 1.0f, 0.0f);
	Z = glm::vec3(0.0f, 0.0f, 1.0f);

	Position = glm::vec3(7.0f, 7.0f, 7.0f);
	Reference = glm::vec3(7.0f, 7.0f, 7.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Debug camera mode: Disabled for the final game (but better keep the code)

	glm::vec3 newPos(0,0,0);
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;
	
	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
	
	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.01f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, glm::vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, glm::vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = glm::vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const  glm::vec3 &Position, const  glm::vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = glm::normalize(Position - Reference);
	X = glm::normalize(cross(glm::vec3(0.0f, 1.0f, 0.0f), Z));
	Y = glm::cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const glm::vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = glm::normalize(cross(glm::vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const glm::vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}
// ------------------------------------------------------------------
void ModuleCamera3D::Move(Direction d, float speed)
{
	glm::vec3 newPos(0, 0, 0);
	switch (d)
	{
	case GO_RIGHT:
		newPos += X * speed;
		break;
	case GO_LEFT:
		newPos -= X * speed;
		break;
	case GO_UP:
		newPos.y += speed;
		break;
	case GO_DOWN:
		newPos.y -= speed;
		break;
	default:
		break;
	}

	Position += newPos;
	Reference += newPos;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return glm::value_ptr(ViewMatrix);
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = glm::mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::Rotate(float x, float y)
{
	int dx = -x;
	int dy = -y;

	float Sensitivity = 0.25f;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, Y);
		Y = rotate(Y, DeltaX, Y);
		Z = rotate(Z, DeltaX, Y);
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = glm::vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	Position = Reference + Z * length(Position);

	CalculateViewMatrix();
}
// -----------------------------------------------------------------

void ModuleCamera3D::From3Dto2D(glm::vec3 point, int& x, int& y)
{
	glm::mat4x4 projection;
	float coty = 1.0f / tan(60.0f * (float)M_PI / 360.0f);
	projection[0][0] = coty / ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
	projection[1][1] = coty;
	projection[2][2] = (0.125f + 512.0f) / (0.125f - 512.0f);
	projection[2][3] = -1.0f;
	projection[3][2] = 2.0f * 0.125f * 512.0f / (0.125f - 512.0f);
	projection[3][3] = 0.0f;

	//glm::vec3 screen = multiply(point, ViewMatrix);
	glm::vec3 screen;
	screen.x = ViewMatrix[0].x * point.x + ViewMatrix[1].x * point.y + ViewMatrix[2].x * point.z + ViewMatrix[3].x;
	screen.y = ViewMatrix[0].y * point.x + ViewMatrix[1].y * point.y + ViewMatrix[2].y * point.z + ViewMatrix[3].y;
	screen.z = ViewMatrix[0].z * point.x + ViewMatrix[1].z * point.y + ViewMatrix[2].z * point.z + ViewMatrix[3].z;
	//screen = multiply(screen, projection);
	screen.x = projection[0].x * screen.x + projection[1].x * screen.y + projection[2].x * screen.z + projection[3].x;
	screen.y = projection[0].y * screen.x + projection[1].y * screen.y + projection[2].y * screen.z + projection[3].y;
	screen.z = projection[0].z * screen.x + projection[1].z * screen.y + projection[2].z * screen.z + projection[3].z;	

	screen.x /= screen.z;
	screen.y /= screen.z;

	x = (screen.x +1) * (SCREEN_WIDTH /2);
	y = (screen.y + 1) * (SCREEN_HEIGHT /2);
}

// -----------------------------------------------------------------

