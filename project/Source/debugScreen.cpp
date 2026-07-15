#ifdef _DEBUG
#include "DebugScreen.h"
#include <stdarg.h>

DebugScreen::DebugScreen(SceneBase * scene) : GameObject(scene)
{
	string.clear();
	color = GetColor(0, 05, 0);
}

DebugScreen::DebugScreen()
{
	string.clear();
	color = GetColor(0, 0, 0);
}

DebugScreen::~DebugScreen()
{
	string.clear();
}

void DebugScreen::Draw()
{
	SetFontSize(14);
	for (auto obj : string) {
		DrawString(obj.x, obj.y, obj.str.c_str(), obj.color);
	}
	string.clear();
}

void DebugScreen::Puts(int x, int y, const char* str)
{
	string.emplace_back(String(x, y, color, str));
}
#endif