#pragma once
#include "Character_Base.h"

class Character00_AttackDS : public Character_AttackState
{
private:
	const int m_HitStopFrame = 6;

public:
	Character00_AttackDS() {};
	~Character00_AttackDS() override {};

	void Init() override;
	void Uninit() override;
	void Update() override;
	void HitCharacter(Character* pHitCharacter) override;

private:
	int m_FrameCount = 0;
	float m_AnimeTime = 0.0f;
	int m_HitStopCount = 0;
};
