#pragma once
#include "Module.h"
#include "Globals.h"

#include <vector>



class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool				Start();
	update_status		Update(float dt);
	bool				CleanUp();

private:
	void				FillBar(Timer &timer, const int &timer_check,vector<float> &container, float new_value);
	void				CloseApp();
	void				MenuMainBar();
	void				MenuFile();
	void				MenuHelp();
	void				MenuView();

public:

private:
	bool				want_to_close = false;
	bool				demo = false;
	bool				configuration = false;

	Timer				frame_timer;
	Timer				ms_timer;
	vector<float>		ms;
	vector<float>		frames;

};
