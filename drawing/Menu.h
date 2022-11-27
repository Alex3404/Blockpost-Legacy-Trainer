#pragma once
#include "../utils/FuncHook.h"

namespace Menu
{
	bool isShowing();
	bool toggleMenu();
	void initialize(HMODULE hmod);
	void renderMenu();
	void renderMods();
};