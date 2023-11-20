#pragma once
#include "Character_Base.h"

class Character_00 : public Character
{
private:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void HitGround() override;
private:
};