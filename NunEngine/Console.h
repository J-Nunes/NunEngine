#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Imgui\imgui.h"

class Console
{
public:
	Console();
	~Console();

	void Draw();

	void Write(const char* text);

	void SetActive();

private:
	ImGuiTextBuffer		buffer;
	bool                ScrollToBottom;
	bool				active = false;
};

extern Console* console;

#endif
