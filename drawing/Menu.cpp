#include "pch-il2cpp.h"
#include "Menu.h"
#include "../utils/kiero.h"
#include "imgui/imgui.h"
#include "impl/d3d11_impl.h"
#include "impl/win32_impl.h"
#include "../memory/MemPatch.h"
#include "../mod/ModManager.h"

static bool isShowingMenu;

bool Menu::isShowing()
{
    return isShowingMenu;
}

bool Menu::toggleMenu()
{
    return (isShowingMenu = !isShowingMenu);
}

void Menu::initialize(HMODULE hmod)
{
    if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success) {
        switch (kiero::getRenderType()) {
            #if KIERO_INCLUDE_D3D11
            case kiero::RenderType::D3D11:
                impl::d3d11::init();
                break;
            #endif
            #if KIERO_INCLUDE_OPENGL
            case kiero::RenderType::OpenGL:
                // TODO: OpenGL implementation?
                break;
            #endif
            #if KIERO_INCLUDE_VULKAN
            case kiero::RenderType::Vulkan:
                // TODO: Vulkan implementation?
                break;
            #endif
        }
    }
#if WIN32
    impl::win32::init(hmod);
#endif
}

void Menu::renderMenu()
{
    ImGui::Begin("Blockpost Hacks");
    ModManager::onMenuRender();
    ImGui::End();
}

void Menu::renderMods()
{
    ModManager::onRender();
}
