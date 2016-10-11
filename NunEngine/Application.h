#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleGeometryLoader.h"
#include "ModuleGameObject.h"

using namespace std; 

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleEditor* editor;
	ModuleGeometryLoader* geometry_loader;
	ModuleGameObject* game_object;

private:
	list<Module*> list_modules;

	PerfTimer			ptimer;
	int					frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	int					last_sec_frame_count = 0;
	int					prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					last_frame_ms = 0;
	float				capped_ms = -1;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* url);
	const int GetFPS();
	const int GetFrameMs();
	bool SecCounter();
	void SetMaxFPS(const uint &fps);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void InitIlu();
};