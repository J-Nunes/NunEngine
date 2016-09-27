#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

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
	ms_timer.Start();

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
	MenuMainBar();
	
	if (want_to_close)
		return UPDATE_STOP;
	else
		return UPDATE_CONTINUE;
}

void ModuleEditor::FillBar(Timer &timer, const int &timer_check, vector<float> &container, float new_value)
{
	if (timer.Read() > timer_check)
	{
		timer.Start();

		if (container.size() > 100)
		{
			for (int i = 1; i < container.size(); i++)
				container[i - 1] = container[i];

			container[container.size() - 1] = new_value;
		}

		else
			container.push_back(new_value);
	}
}

void ModuleEditor::CloseApp()
{
	want_to_close = true;
}

void ModuleEditor::MenuMainBar()
{
	ImGui::BeginMainMenuBar();

	MenuFile();
	MenuHelp();
	MenuView();

	ImGui::EndMainMenuBar();
}

void ModuleEditor::MenuFile()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit"))
			CloseApp();

		ImGui::EndMenu();
	}
}

void ModuleEditor::MenuHelp()
{
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

	if (demo)
		ImGui::ShowTestWindow();
}

void ModuleEditor::MenuView()
{
	FillBar(frame_timer, 1000, frames, App->GetFPS());
	FillBar(ms_timer, 1, ms, App->GetFrameMs());

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Configuration"))
			configuration = !configuration;

		ImGui::EndMenu();
	}

	if (configuration)
	{
		ImGui::Begin("Configuration");

		if (ImGui::CollapsingHeader("Application"))
		{
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", frames[frames.size() - 1]);
			ImGui::PlotHistogram("##framerate", &frames[0], frames.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %.1f", ms[ms.size() - 1]);
			ImGui::PlotHistogram("##framerate", &ms[0], ms.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		}

		ImGui::End();
	}
}