#include "Character_Base.h"
#include "Attack_Base.h"
#include "Scene.h"
#include "00_SceneGame.h"

int Character::m_NewPlayerBit = 0x01;

void Character::InitPlayerBit()
{
	m_NewPlayerBit = 0x01;
}

int Character::GetNewPlayerBit()
{
	int ReturnBit = m_NewPlayerBit;
	m_NewPlayerBit = m_NewPlayerBit << 1;
	return ReturnBit;
}

void Character::Character_Init()
{
	m_PlayerBit = GetNewPlayerBit();
	int PlayerNum = static_cast<int>(log2f(static_cast<float>(m_PlayerBit))) + 1;

	//キャラクターの番号によってステージの場所を変える
	std::vector<CVector3>* startPosV = static_cast<SceneGame*>(CScene::GetScene())->GetStage()->GetCharacterStartPos();
	m_pos = (*startPosV)[PlayerNum - 1];
	
	Init();

	m_ChangeState = false;
	m_JumpCount = 0;
	m_HitGround = m_HitCeiling = m_HitWall = false;

	switch (m_NowState)
	{
	case Character::STATE::IDLE:
		IdleInit();
		break;
	case Character::STATE::WALK:
		WalkInit();
		break;
	case Character::STATE::DASH:
		DashInit();
		break;
	case Character::STATE::ATTACK:
		AttackInit();
		break;
	case Character::STATE::BLOWAWAY:
		BlowAwayInit();
		break;
	case Character::STATE::JUMPIN:
		JumpInInit();
		break;
	case Character::STATE::JUMP:
		JumpInit();
		break;
	case Character::STATE::AIRMOVE:
		AirMoveInit();
		break;
	case Character::STATE::DOWN:
		DownInit();
		break;
	case Character::STATE::MAX:
		break;
	default:
		break;
	}
}

void Character::Character_Uninit()
{
	Uninit();
}

void Character::Character_Update()
{
	m_oldPos = m_pos;

	Update();

	//========================================
	// 各ステータスのアップデート
	//========================================
	switch (m_NowState)
	{
	case Character::STATE::IDLE:
		IdleUpdate();
		break;
	case Character::STATE::WALK:
		WalkUpdate();
		break;
	case Character::STATE::DASH:
		DashUpdate();
		break;
	case Character::STATE::ATTACK:
		AttackUpdate();
		break;
	case Character::STATE::BLOWAWAY:
		BlowAwayUpdate();
		break;
	case Character::STATE::JUMPIN:
		JumpInUpdate();
		break;
	case Character::STATE::JUMP:
		JumpUpdate();
		break;
	case Character::STATE::AIRMOVE:
		AirMoveUpdate();
		break;
	case Character::STATE::DOWN:
		DownUpdate();
		break;
	case Character::STATE::MAX:
		break;
	default:
		break;
	}

	//========================================
	// 各ステータスが切り替わっていたら
	//========================================
	if (m_ChangeState)
	{
		m_ChangeState = false;

		//========================================
		//終了処理
		//========================================
		switch (m_NowState)
		{
		case Character::STATE::IDLE:
			IdleUninit();
			break;
		case Character::STATE::WALK:
			WalkUninit();
			break;
		case Character::STATE::DASH:
			DashUninit();
			break;
		case Character::STATE::ATTACK:
			AttackUninit();
			break;
		case Character::STATE::BLOWAWAY:
			BlowAwayUninit();
			break;
		case Character::STATE::JUMPIN:
			JumpInUninit();
			break;
		case Character::STATE::JUMP:
			JumpUninit();
			break;
		case Character::STATE::AIRMOVE:
			AirMoveUninit();
			break;
		case Character::STATE::DOWN:
			DownUninit();
			break;
		case Character::STATE::MAX:
			break;
		default:
			break;
		}

		//========================================
		// 初期化
		//========================================
		switch (m_NextState)
		{
		case Character::STATE::IDLE:
			IdleInit();
			break;
		case Character::STATE::WALK:
			WalkInit();
			break;
		case Character::STATE::DASH:
			DashInit();
			break;
		case Character::STATE::ATTACK:
			AttackInit();
			break;
		case Character::STATE::BLOWAWAY:
			BlowAwayInit();
			break;
		case Character::STATE::JUMPIN:
			JumpInInit();
			break;
		case Character::STATE::JUMP:
			JumpInit();
			break;
		case Character::STATE::AIRMOVE:
			AirMoveInit();
			break;
		case Character::STATE::DOWN:
			DownInit();
			break;
		case Character::STATE::MAX:
			break;
		default:
			break;
		}

		m_NowState = m_NextState;

	}

	m_CharacterCollider.SetPos(m_pos);

	m_DamageUI.GetDamageUI()->SetNumber(static_cast<int>(m_DamagePercentage));
	m_DamageUI.Update();
}

void Character::Character_Draw()
{
	Draw();

	m_CharacterModel.SetPosition(m_pos);
	m_CharacterModel.SetRotate(m_rotate);
	m_CharacterModel.Draw();
}

void Character::Character_UIDraw()
{
	m_DamageUI.Draw();
}

int Character::GetCharacterBit()
{
	return m_PlayerBit;
}

const Character::STATE& Character::GetState() const
{
	return m_NowState;
}

const Character::ATTACK& Character::GetAttack() const
{
	return m_NowAttack;
}

ModelDrawer* Character::GetModel() const
{
	return const_cast<ModelDrawer*>(&m_CharacterModel);
}

const CVector3& Character::GetPos() const
{
	return m_pos;
}

void Character::SetPos(const CVector3 & pos)
{
	m_pos = pos;
	m_CharacterCollider.SetPos(pos);
}

const CVector3& Character::GetOldPos() const
{
	return m_oldPos;
}

void Character::SetOldPos()
{
	m_oldPos = m_pos;
}

const CVector3 & Character::GetScale() const
{
	return m_scale;
}

void Character::SetScale(const CVector3 & scale)
{
	m_scale = scale;
}

const CQuaternion & Character::GetRotate() const
{
	return m_rotate;
}

void Character::SetRotate(const CQuaternion & rotate)
{
	m_rotate = rotate;
}

void Character::SetRotate(const CVector3 & rotate)
{
	m_rotate.v = DirectX::XMQuaternionRotationRollPitchYaw(
		DirectX::XMConvertToRadians(rotate.x),
		DirectX::XMConvertToRadians(rotate.y),
		DirectX::XMConvertToRadians(rotate.z));
}

void Character::AddForce(const CVector3 & force)
{
	m_Velocity += force;
}

void Character::SetForce(const CVector3 & force)
{
	m_Velocity = force;
}

float Character::GetDamage() const
{
	return m_DamagePercentage;
}

void Character::AddDamage(float damage)
{
	m_DamagePercentage += damage;
}

void Character::SetDamage(float damage)
{
	m_DamagePercentage = damage;
}

BoxCollider* Character::GetCharacterCollider() const
{
	return const_cast<BoxCollider*>(&m_CharacterCollider);
}

std::vector<Character::AttackParam>& Character::GetAttackCollider()
{
	return m_AttackCollider;
}

void Character::Character_ColliderInit()
{
	m_HitCeiling = m_HitGround = m_HitWall = false;
}

void Character::Character_HitCeiling()
{
	m_HitCeiling = true;
	HitCeiling();
}

void Character::Character_HitGround()
{
	m_Velocity.y = 0.0f;
	m_JumpCount = 0;
	m_HitGround = true;
	HitGround();
}

void Character::Character_HitWall()
{
	m_HitWall = true;
	HitWall();
}

void Character::DrawCollider()
{
	m_CharacterCollider.DrawCollider();
}

void Character::ChangeAttack(Character::ATTACK attack)
{
	ChangeState(Character::STATE::ATTACK);
	m_NowAttack = attack;
}

void Character::ChangeState(Character::STATE state)
{
	m_NextState = state;
	m_ChangeState = true;
}

