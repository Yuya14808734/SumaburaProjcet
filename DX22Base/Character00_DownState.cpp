#include "Character00_DownState.h"

void Character00_DownState::Init()
{
	CharacterBase_DownState::Init();

	m_pModelDrawer->PlayAnime("Damage01", true);
	m_AnimeTime = 0.394f;
	m_FrameCount = 0;
	m_pCharacter->SetInvincible(true);

	//=====<キャラクターの当たり判定の調整>============
	BoxCollider* pCollider = m_pCharacter->GetCharacterCollider();

	CVector3 copysize = pCollider->GetSize();

	pCollider->SetSize(CVector3(
		copysize.y,	//yと
		copysize.x,	//xを入れ替えている
		copysize.z
	));

	pCollider->SetShiftVec(CVector3::GetRight() *
		(copysize.y * (m_pCharacter->GetLook() == Character::LOOKDIR::RIGHT ? -0.5f : 0.5f)
		));
	//=================================================

}

void Character00_DownState::Uninit()
{
	CharacterBase_DownState::Uninit();
}

void Character00_DownState::Update()
{
	CharacterBase_DownState::Update();

	const int AnimeEndFrame = 20;			//アニメーションを停止するフレーム
	const float AnimeStartTime = 0.394f;		//アニメーションの始める時間
	const float AnimeEndTime = 1.461f;		//アニメーションを終わる時間
	const float AnimeAllTime = AnimeEndTime - AnimeStartTime;	//アニメーションの時間

	m_FrameCount++;

	m_AnimeTime = static_cast<float>(m_FrameCount) / static_cast<float>(AnimeEndFrame) * AnimeAllTime + AnimeStartTime;

	//アニメーションが終了したら待機状態にする
	if (m_AnimeTime > AnimeEndTime)
	{
		m_AnimeTime = AnimeEndTime;

		//転んでいるときに上ボタンを押すと起き上がる
		if (m_pController->GetWakeUp())
		{
			m_pCharacter->SetNextState(Character::STATE::State_WakeUp);
		}
	}

	//アニメーションが終了してある程度時間がたったら無敵を外す
	if (m_FrameCount > AnimeEndFrame + 30)
	{
		m_pCharacter->SetInvincible(false);
	}

	m_pModelDrawer->SetAnimeTime(m_AnimeTime);
}