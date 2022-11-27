#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include "Aimbot.h"
#include "helpers.h"
#include "../drawing/imgui/imgui.h"
#include <Windows.h>

using namespace app;

bool aimBot = false;

Vector3 getPlayerHeadPosition(PlayerData* player)
{
    if (player && player->fields.goHead) {
        auto transform = GameObject_get_transform(player->fields.goHead, nullptr);
        if (transform)
            return Transform_get_position(transform, nullptr);
    }
    return {};
}

Vector3 calcAngles(Vector3 delta)
{
    Quaternion direction = Quaternion_LookRotation_1(delta, nullptr);
    Vector3 eularAngles = Quaternion_Internal_ToEulerRad(direction, nullptr);
    eularAngles = Vector3_op_Multiply(eularAngles, (float)(360 / (3.14159265358979323846 * 2)), nullptr);
    return eularAngles;
}

void Aimbot::onUpdate()
{
    if (aimBot && Input_GetMouseButton(1, nullptr)) {
        if (il2cppi_is_initialized(PLH__TypeInfo) && il2cppi_is_initialized(Controll__TypeInfo)) {
            auto staticFields = (*Controll__TypeInfo)->static_fields;

            auto players = (*PLH__TypeInfo)->static_fields->player;
            auto mainPlayer = staticFields->pl;
            auto camPostion = Transform_get_position(staticFields->trCamera, nullptr);
            auto currentAngles = Vector3 { staticFields->ry, staticFields->rx, 0 };

            PlayerData* closestEnemy = nullptr;
            double angleDistance = DBL_MAX;
            if (players) {
                for (int i = 0; i < players->max_length; i++) {
                    auto player = players->vector[i];
                    if (player) {
                        if (!PlayerData_get_IsMainPlayer(player, nullptr)) {
                            Vector3 directionToEnemy = Vector3_op_Subtraction(getPlayerHeadPosition(player), camPostion, nullptr);
                            Vector3 anglesToEnemy = calcAngles(directionToEnemy);
                            Vector3 deltaAngles = Vector3_op_Subtraction(anglesToEnemy, currentAngles, nullptr);
                            float angleDistanceToPlayer = Vector3_Magnitude(deltaAngles, nullptr);
                            float distanceToPlayer = Vector3_Magnitude(directionToEnemy, nullptr);

                            // todo check if they are invuraible 
                            if (angleDistance > angleDistanceToPlayer && player->fields.health > 0 && player->fields.team != mainPlayer->fields.team) {
                                // Attempts to ray cast to the player and checks if we hit a player (we still need to check if its an enemy player)
                                RaycastHit hit;
                                if (Controll_Bulletcast(camPostion, directionToEnemy, &hit, distanceToPlayer, nullptr)) {
                                    auto object = reinterpret_cast<Transform * (*)(RaycastHit*)>((il2cppi_get_base_address() + 0x667910))(&hit);
                                    if (object) {
                                        __try {
                                            auto gameObject = Component_1_get_gameObject((Component_1*)object, nullptr);
                                            auto hitData = Component_1_GetComponent_22((Component_1*)object, *Component_1_GetComponent_22__MethodInfo);
                                            if (hitData) {
                                                angleDistance = angleDistanceToPlayer;
                                                closestEnemy = player;
                                            }
                                        }
                                        __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION
                                                ? EXCEPTION_EXECUTE_HANDLER
                                                : EXCEPTION_CONTINUE_SEARCH) {

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (closestEnemy) {
                // Get angles to the player
                Vector3 deltaVec = Vector3_op_Subtraction(getPlayerHeadPosition(closestEnemy), camPostion, nullptr);
                Vector3 angles = calcAngles(deltaVec);

                // Forces to look at one point
                staticFields->ry = angles.x;
                staticFields->rx = angles.y;
            }
        }
    }
}

void Aimbot::onMenuRender()
{
    ImGui::Checkbox("Aimbot", &aimBot);
}

void Aimbot::onRender()
{
}

void Aimbot::initialize()
{

}
