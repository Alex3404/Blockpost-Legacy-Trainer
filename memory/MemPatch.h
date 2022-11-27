#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class MemPatch
{
private:
	bool status;
	char** oldBytes;
	size_t size;
	DWORDLONG address;
	const char* patch;
public:
	MemPatch(DWORDLONG address, const char* patch, size_t size);
	void Enable();
	void Disable();
};

