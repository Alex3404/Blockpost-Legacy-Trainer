#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "../drawing/imgui/imgui.h"

#include "GunHacks.h"

#include "../../memory/MemPatch.h"
#include "../../memory/Memory.h"
#include "../../utils/FuncHook.h"

using namespace app;

bool noRecoil = false;
bool noBulletSpread = false;

void ToggleBulletSpread(bool enabled)
{
    static MemPatch* memPatch;
    if (!memPatch) {
        // Sig scanning for
        //GameAssembly.dll+6AFCEA - B9 64000000           - mov ecx,00000064 { 100 }
        //GameAssembly.dll+6AFCEF - 8B C1                 - mov eax,ecx
        //GameAssembly.dll+6AFCF1 - 2B 46 28              - sub eax,[esi+28]
        //GameAssembly.dll+6AFCF4 - 66 0F 6E C8            - movd xmm1,eax
        //GameAssembly.dll+6AFCF8 - 0F5B C9               - cvtdq2ps xmm1,xmm1
        //GameAssembly.dll+6AFCFB - 8B 46 08              - mov eax,[esi+08]
        DWORDLONG address = (DWORDLONG)Memory::PatternScan(GetModuleHandleA("GameAssembly.dll"),
            "B9 64 00 00 00 8B C1 2B 46 28 66 0F 6E C8 0F 5B C9 8B 46 08");

#if DEBUG
        if (!address) {
            std::cout << "Failed to find pattern for no bullet spread!" << std::endl;
            return;
        }
#endif

        // A patch that changes the assembly code to make the accuracy value become 100%
        // GameAssembly.dll + 6AFCEA - B9 64 00 00 00 - mov ecx, 00000064 { 100 }
        // GameAssembly.dll + 6AFCEF - 8B C1 - mov eax, ecx
        // GameAssembly.dll + 6AFCF1 - 2B 46 28 - sub eax, [esi + 28]
        // Replaced with
        //GameAssembly.dll + 6AFCEA - B8 00 00 00 00 - mov eax, 00000000 { 0 }
        //GameAssembly.dll + 6AFCEF - B9 64 00 00 00 - mov ecx, 00000064 { 100 }
        static const char patch[] = { '\xB8', '\x00' ,'\x00', '\x00', '\x00', '\xB9', '\x64', '\x00', '\x00', '\x00' };
        memPatch = new MemPatch(address, patch, sizeof(patch));
    }

    if (memPatch) {
        if (enabled) memPatch->Enable();
        else memPatch->Disable();
    }
}


void ToggleNoRecoil(bool enabled)
{
    static MemPatch* memPatch;
    if (!memPatch) {
        //GameAssembly.dll + 6AE6B6 - E8 65DE1E00 - call GameAssembly.dll + 89C520
        //GameAssembly.dll + 6AE6BB - 83 C4 1C - add esp, 1C
        //GameAssembly.dll + 6AE6BE - C7 45 A0 00000000 - mov[ebp - 60], 00000000
        //GameAssembly.dll + 6AE6C5 - 8D 45 98 - lea eax, [ebp - 68]
        //GameAssembly.dll + 6AE6C8 - 0F57 C0 - xorps xmm0, xmm0
        DWORDLONG address = (DWORDLONG)Memory::PatternScan(GetModuleHandleA("GameAssembly.dll"),
            "E8 ? ? ? ? 83 C4 1C C7 45 A0 00 00 00 00 8D 45 98 0F 57 C0");

#if DEBUG
        if (!address) {
            std::cout << "Failed to find pattern for no recoil!" << std::endl;
            return;
        }
#endif

        // GameAssembly.dll+6AE6B6 - E8 65DE1E00           - call GameAssembly.dll+89C520
        // Replacing with
        // NOPS
        static const char patch[] = { '\x90', '\x90' ,'\x90', '\x90', '\x90' };
        memPatch = new MemPatch(address, patch, sizeof(patch));
    }
    if (memPatch) {
        if (enabled) memPatch->Enable();
        else memPatch->Disable();
    }
}

FuncHook* setRecoilHook;
void hook_Controll_SetRecoil(Controll* __this, int32_t recoiltype, float force, MethodInfo* method)
{
    if (noRecoil)
        return;

    setRecoilHook->GetCall<void, Controll*, int32_t, float, MethodInfo*>()(__this, recoiltype, force, method);
}

bool lastNoBulletSpread = false;
bool lastNoRecoil = false;
void GunHacks::onUpdate()
{
    if (lastNoBulletSpread != noBulletSpread)
        ToggleBulletSpread(noBulletSpread);
    if (lastNoRecoil != noRecoil)
        ToggleBulletSpread(noRecoil);

    lastNoBulletSpread = noBulletSpread;
    lastNoRecoil = noRecoil;
}

void GunHacks::onMenuRender()
{
    ImGui::Checkbox("No Bullet Spread", &noBulletSpread);
    ImGui::Checkbox("No Recoil", &noRecoil);
}

void GunHacks::onRender()
{

}

void GunHacks::initialize()
{
    setRecoilHook = new FuncHook(Controll_SetRecoil, hook_Controll_SetRecoil);
    setRecoilHook->Enable();
}
