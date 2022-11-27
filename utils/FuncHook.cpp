#include "pch-il2cpp.h"
#include "FuncHook.h"

FuncHook::FuncHook(void* func, void* newFunc)
    : originalFunc(func)
{
    if (MH_CreateHook(func, newFunc, &realFunctionPtr) != MH_OK) {
#if DEBUG
        std::cout << "Failed to create hook!" << std::endl;
#endif
    }
}

FuncHook::~FuncHook()
{
    if (MH_RemoveHook(originalFunc) != MH_OK) {
#if DEBUG
        std::cout << "Failed to remove hook!" << std::endl;
#endif
    }
}

void FuncHook::Enable()
{
    if (MH_EnableHook(originalFunc) != MH_OK) {
#if DEBUG
        std::cout << "Failed to enable hook!" << std::endl;
#endif
    }
}

void FuncHook::Disable()
{
    if (MH_DisableHook(originalFunc) != MH_OK) {
#if DEBUG
    std::cout << "Failed to disable hook!" << std::endl;
#endif
    }
}