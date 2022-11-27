#include "pch-il2cpp.h"
#include "MemPatch.h"
#include "Memory.h"

MemPatch::MemPatch(DWORDLONG address, const char* patch, size_t size)
	: address(address), patch(patch), status(false), size(size)
{
    // Create the buffer to hold the old bytes
    oldBytes = (char**)malloc(size);
    
    // Change the memory permissions to allow read and writing
    DWORD oldMemoryProtection, _;
    VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldMemoryProtection);
    // Read the bytes and store them as the old bytes
    memcpy(oldBytes, reinterpret_cast<void*>(address), size);
    // Restore the old memory permissions
    VirtualProtect((LPVOID)address, size, oldMemoryProtection, &_);
}

void MemPatch::Enable()
{
    if (status) return;
    status = true;

    // Change the memory permissions to allow read and writing
    DWORD oldMemoryProtection, _;
    VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldMemoryProtection);
    // Patch the bytes
    memcpy(reinterpret_cast<void*>(address), (void*)patch, size);
    // Restore the old memory permissions
    VirtualProtect((LPVOID)address, size, oldMemoryProtection, &_);
}

void MemPatch::Disable()
{
    if (status) return;
    status = false;

    // Change the memory permissions to allow read and writing
    DWORD oldMemoryProtection, _;
    VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldMemoryProtection);
    // Patch the bytes
    memcpy(reinterpret_cast<void*>(address), (void*)*oldBytes, size);
    // Restore the old memory permissions
    VirtualProtect((LPVOID)address, size, oldMemoryProtection, &_);
}
