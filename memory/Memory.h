#pragma once

#include <sstream>
#include <vector>
#include "Shlwapi.h"
#include <psapi.h>
#include <Windows.h>
#include <tlhelp32.h>

namespace Memory
{
	void* PatternScan(HMODULE module, const char* signature);
};