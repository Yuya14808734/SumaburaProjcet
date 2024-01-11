#include "CharacterBase_AirMoveState.h"
#include "Input.h"
#include "XboxKeyboard.h"

void CharacterBase_AirMoveState::Init()
{
	m_pCharacterParameter->MoveVector = CVector3::GetZero();
}

void CharacterBase_AirMoveState::Uninit()
{

}

void CharacterBase_AirMoveState::Update()
{
	if (m_pCharacterParameter->HitGround)
	{
		m_pCharacter->SetNextState(Character::STATE::State_Idle);
	}

	bool OnButton = false;

	float LeftStickX = m_pController->GetMoveInput().x * m_pMoveParameter->AirSideMoveSpeed;
	m_pCharacterParameter->Velocity.x = 
		fabsf(LeftStickX) > fabsf(m_pCharacterParameter->Velocity.x) ? LeftStickX : m_pCharacterParameter->Velocity.x;

	//もう一度ジャンプができる
	if (m_pCharacterParameter->JumpCount < m_pJumpParameter->MaxJumpCount)
	{
		if (m_pController->GetJumpTrigger())
		{
			m_pCharacterParameter->JumpCount++;
			m_pCharacterParameter->Velocity.y = m_pJumpParameter->FirstJumpPower;
			m_pCharacter->SetNextState(Character::STATE::State_Jump);
		}
	}

	//攻撃
	if (m_pController->GetAttack())
	{
		m_pCharacter->SetNextState(Character::STATE::State_AttackAirN);//弱の設定
	}

	if (!OnButton)
	{
		m_pCharacterParameter->Velocity.x *= m_pMoveParameter->AirDrag;	//空気抵抗を掛ける
	}

	m_pCharacterParameter->Velocity.y += m_pJumpParameter->FallDownGravity;			//重力を掛ける

	//重力制御(最大の落下速度になったら)
	if (m_pCharacterParameter->Velocity.y < m_pJumpParameter->DefaultFallSpeed)
	{
		m_pCharacterParameter->Velocity.y = m_pJumpParameter->DefaultFallSpeed;
	}

	m_pCharacterParameter->Pos += m_pCharacterParameter->MoveVector;
	m_pCharacterParameter->Pos += m_pCharacterParameter->Velocity;
}