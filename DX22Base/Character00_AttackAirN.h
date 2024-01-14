#pragma once
#include "Character_Base.h"
#include "Effect_Manager.h"

class Character00_AttackAirN : public Character_AttackState
{
private:
	const int m_HitStopFrame = 30;

public:
	Character00_AttackAirN() {};
	~Character00_AttackAirN() override {};

	void Init() override;
	void Uninit() override;
	void Update() override;
	void HitCharacter(Character* pHitCharacter) override;

private:
	int m_FrameCount = 0;
	float m_AnimeTime = 0.0f;
	bool m_PushButton = false;
	int m_HitAttackStopCount = 0;
	Effekseer::Handle m_AirNEffect;
};
