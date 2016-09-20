#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "Imgui\imgui.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;



	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleEditor::Update(float dt)
{
	//Create the menu bar
	ImGui::BeginMainMenuBar();
	
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Demo"))
			demo = !demo;

		if (ImGui::MenuItem("Documentation"))
			App->RequestBrowser("https://github.com/J-Nunes/NunEngine/wiki");

		if (ImGui::MenuItem("Download latest version"))
			App->RequestBrowser("https://github.com/J-Nunes/NunEngine/releases");

		if (ImGui::MenuItem("Report a bug"))
			App->RequestBrowser("https://github.com/J-Nunes/NunEngine/issues");

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if(demo)
		ImGui::ShowTestWindow();

	
	
	return UPDATE_CONTINUE;
}