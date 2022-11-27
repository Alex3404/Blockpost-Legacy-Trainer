#pragma once
class Mod
{
public:
	virtual void initialize();
	virtual void onUpdate();
	virtual void onMenuRender();
	virtual void onRender();
};