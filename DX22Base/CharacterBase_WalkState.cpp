#include "CharacterBase_WalkState.h"

void CharacterBase_WalkState::Init()
{
	m_pCharacterParameter->Velocity.x = 0.0f;
	m_WalkTimeCount = 0;
}

void CharacterBase_WalkState::Uninit()
{

}

void CharacterBase_WalkState::Update()
{
	m_WalkTimeCount++;

	//地面に当たっていなければ
	if (!m_pCharacterParameter->HitGround)
	{
		m_pCharacter->SetNextState(Character::STATE::State_AirMove);
	}

	if (m_WalkTimeCount < 2)
	{
		if (m_pController->GetLeftSmash())
		{
			m_pCharacter->SetNextState(Character::STATE::State_Dash);
		}
	}

	bool NoButton = true;

	m_pCharacterParameter->Velocity.x 
		= m_pController->GetMoveInput().x * m_pMoveParameter->WalkSpeed;

	if (m_pCharacterParameter->Velocity.x != 0.0f)
	{
		NoButton = false;

		if (m_pCharacterParameter->Velocity.x > 0.0f)
		{
			//右に行っている
			m_pCharacter->SetLookRight();
		}
		else
		{
			//左に行っている
			m_pCharacter->SetLookLeft();
		}

	}

	//ボタンが何も押されていなければ
	if (NoButton)
	{
		m_pCharacter->SetNextState(Character::STATE::State_Idle);
	}

	//攻撃
	if (m_pController->GetAttack())
	{
		m_pCharacter->SetNextState(Character::STATE::State_AttackS2);
	}

	//ジャンプ
	if (m_pController->GetJumpTrigger())
	{
		m_pCharacter->SetNextState(Character::STATE::State_JumpIn);
	}

	m_pCharacterParameter->Velocity.y += m_pJumpParameter->FallDownGravity;

	//一応重力制御も書いておく
	if (m_pCharacterParameter->Velocity.y < m_pJumpParameter->DefaultFallSpeed)
	{
		m_pCharacterParameter->Velocity.y = m_pJumpParameter->DefaultFallSpeed;
	}

	m_pCharacterParameter->Pos += m_pCharacterParameter->Velocity;
}