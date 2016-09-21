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

	frame_timer.Start();

	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleEditor::FillFPSBar()
{
	if (frame_timer.Read() > 1000)
	{
		frame_timer.Start();

		if (frames_to_print.size() > 100)
		{
			for (int i = 1; i < frames_to_print.size(); i++)
				frames_to_print[i - 1] = frames_to_print[i];

			frames_to_print[frames_to_print.size() - 1] = App->GetFPS();
		}

		else
			frames_to_print.push_back(App->GetFPS());
	}
}

// Update
update_status ModuleEditor::Update(float dt)
{
	FillFPSBar();

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

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Configuration"))
			configuration = !configuration;

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if(demo)
		ImGui::ShowTestWindow();

	if (configuration)
	{
		ImGui::Begin("Configuration");

		if(ImGui::CollapsingHeader("Application"))
		{
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", frames_to_print[frames_to_print.size() - 1]);
			ImGui::PlotHistogram("##framerate", &frames_to_print[0], frames_to_print.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100)); 
		}

		ImGui::End();
	}
	
	return UPDATE_CONTINUE;
}