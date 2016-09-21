#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <vector>



class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool			Start();
	update_status	Update(float dt);
	bool			CleanUp();

private:
	void			FillFPSBar();

public:

private:
	bool				demo = false;
	bool				configuration = false;

	Timer				frame_timer;
	vector<float>		frames_to_print;

};
