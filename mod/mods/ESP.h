#pragma once
#include "../Mod.h"

class ESP : Mod
{
	void onUpdate() override;
	void onMenuRender() override;
	void onRender() override;
	void initialize() override;
};