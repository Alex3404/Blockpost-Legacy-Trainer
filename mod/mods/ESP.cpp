#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "../drawing/imgui/imgui.h"

#include "ESP.h"

#include "../../memory/MemPatch.h"
#include "../../memory/Memory.h"
#include "../../utils/FuncHook.h"
#include <stack>

using namespace app;

bool espEnabled = false;
bool showHealth = false;
bool showTeammates = false;

struct BoxESP
{
    ImVec2 bottomCorner;
    ImVec2 topCorner;
    ImU32 color;
    // std::u16string name;
    float healthPrecentage;
};
std::stack<BoxESP> drawList;

Vector3 getScreenPosition(Vector3 worldPoint)
{
    if (il2cppi_is_initialized(Controll__TypeInfo)) {

        auto staticFields = (*Controll__TypeInfo)->static_fields;
        return Camera_WorldToScreenPoint_1(staticFields->csCam, worldPoint, nullptr);
    }
    return {};
}

void ESP::onUpdate()
{
    if (espEnabled) {
        if (il2cppi_is_initialized(PLH__TypeInfo) && il2cppi_is_initialized(Controll__TypeInfo)) {
            auto staticFields = (*Controll__TypeInfo)->static_fields;

            auto players = (*PLH__TypeInfo)->static_fields->player;
            auto mainPlayer = staticFields->pl;
            auto mainCamera = staticFields->csCam;

            if (players && mainCamera) {
                for (int i = 0; i < players->max_length; i++) {
                    auto player = players->vector[i];
                    if (player) {
                        if (!PlayerData_get_IsMainPlayer(player, nullptr)) {
                            bool teammate = mainPlayer->fields.team == player->fields.team;
                            if (showTeammates || !teammate) {
                                auto boxColor = teammate ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255);

                                if (player->fields.goHead) {
                                    // Get Box Collider
                                    auto headCollider = GameObject_GetComponent_28(player->fields.goHead, *GameObject_GetComponent_28__MethodInfo);
                                    auto bounds = Collider_get_bounds((Collider*)headCollider, nullptr);

                                    auto playerTop = Transform_get_position(GameObject_get_transform(player->fields.goHead, nullptr), nullptr);
                                    playerTop.y += bounds.m_Extents.y * 2;
                                    auto playerBottom = playerTop;
                                    playerTop.y -= 3;

                                    Vector3 screenPosTop = getScreenPosition(playerTop);
                                    Vector3 screenPosBottom = getScreenPosition(playerBottom);
                                    int boxHeight = abs(screenPosBottom.y - screenPosTop.y);

                                    int screenHeight = Screen_get_height(nullptr);

                                    if (screenPosTop.z > 0 && screenPosBottom.y > 0) {
                                        drawList.push({
                                            ImVec2(screenPosTop.x - boxHeight * 0.2f, screenHeight - screenPosTop.y),
                                            ImVec2(screenPosTop.x + boxHeight * 0.2f, screenHeight - screenPosBottom.y),
                                            boxColor,
                                            // std::u16string(reinterpret_cast<const char16_t*>(reinterpret_cast<Il2CppString*>(player->fields.name)->chars)),
                                            player->fields.health / 100.0f,
                                            });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ESP::onMenuRender()
{
    ImGui::Checkbox("ESP", &espEnabled);
    if (espEnabled) {
        ImGui::Indent(16);
        ImGui::Checkbox("Show healthboxes", &showHealth);
        ImGui::Checkbox("Show teammates", &showTeammates);
    }
}

void ESP::onRender()
{
    auto imguiDrawList = ImGui::GetForegroundDrawList();
    while (!drawList.empty()) {
        BoxESP drawCommand = drawList.top();
        drawList.pop();

        imguiDrawList->AddRect(drawCommand.bottomCorner, drawCommand.topCorner, drawCommand.color);
        if (showHealth) {
            ImVec2 healthBarBackTopCorner = ImVec2(drawCommand.bottomCorner.x - 3, drawCommand.topCorner.y);
            ImVec2 healthBarTopCorner = ImVec2(drawCommand.bottomCorner.x - 3,
                drawCommand.bottomCorner.y + ((drawCommand.topCorner.y - drawCommand.bottomCorner.y) * drawCommand.healthPrecentage));
            imguiDrawList->AddRect(drawCommand.bottomCorner, healthBarBackTopCorner, drawCommand.color);
            imguiDrawList->AddRectFilled(drawCommand.bottomCorner, healthBarBackTopCorner, IM_COL32(0, 0, 0, 255));
            imguiDrawList->AddRectFilled(drawCommand.bottomCorner, healthBarTopCorner, IM_COL32(0, 255, 0, 255));
        }
    }
}

void ESP::initialize()
{
}