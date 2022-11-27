// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"

#if _WIN64
#pragma comment(lib, "libMinHook-x64.lib")
#else
#pragma comment(lib, "libMinHook-x86.lib")
#endif

#include <MinHook.h>
#include "../utils/FuncHook.h"
#include "../memory/Memory.h"
#include "../memory/MemPatch.h"
#include "../drawing/Menu.h"
#include "../mod/ModManager.h"

using namespace app;

FuncHook* updateHook;
void hook_Controll_Update(Controll* __this, MethodInfo* method)
{
    if (Input_GetKeyDown(KeyCode__Enum::Home, nullptr))
        Menu::toggleMenu();
    ModManager::onUpdate();
    updateHook->GetCall<void, Controll*, MethodInfo*>()(__this, method);
}

// Setting log file name
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";
void Run(HMODULE hMod)
{
    auto thread = il2cpp_thread_attach(il2cpp_domain_get());

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK) {
        std::cout << "Failed to initalize minHook" << std::endl;
        return;
    }

    Menu::initialize(hMod);
    ModManager::initialize();

    updateHook = new FuncHook(Controll_Update, hook_Controll_Update);
    updateHook->Enable();
}