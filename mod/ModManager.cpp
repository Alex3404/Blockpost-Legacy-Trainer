#include "pch-il2cpp.h"
#include "ModManager.h"
#include "Mod.h"
#include <vector>
#include "mods/Aimbot.h"
#include "mods/GunHacks.h"
#include "mods/ESP.h"

std::vector<Mod*> mods;

void ModManager::initialize()
{
	mods = {
		(Mod*) new Aimbot(),
		(Mod*) new GunHacks(),
		(Mod*) new ESP(),
	};
	for (Mod* mod : mods) {
		mod->initialize();
	}
}

void ModManager::onUpdate()
{
	for (Mod* mod : mods) {
		mod->onUpdate();
	}
}

void ModManager::onMenuRender()
{
	for (Mod* mod : mods) {
		mod->onMenuRender();
	}
}

void ModManager::onRender()
{
	for (Mod* mod : mods) {
		mod->onRender();
	}
}
