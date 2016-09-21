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
	void			FillBar(Timer &timer, const int &timer_check,vector<float> &container, float new_value);

public:

private:
	bool				demo = false;
	bool				configuration = false;

	Timer				frame_timer;
	Timer				ms_timer;
	vector<float>		ms;
	vector<float>		frames;

};
