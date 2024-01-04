#include "Scene00_Game.h"
#include "CameraManager.h"
#include "Camera00_Debug.h"
#include "Camera01_Game.h"
#include "Camera03_GameStart.h"
#include "ModelDrawer.h"
#include "Geometory.h"
#include "Input.h"
#include "Stage_00.h"
#include "Character_00.h"
#include "Player_Controller.h"
#include "Select00_Character.h"
#include "Scene01_Select.h"

bool ColliderDraw = false;

void SceneGame::Init()
{
	//=====<ゲーム開始に設定>=====
	m_GameState = GAMESTATE::GAMESTART;

	//=====<カメラの生成>=====
	CameraManager::GetInstance().CreateCamera(new CameraGame(), "GameCamera");
	CameraGameStart* pCameraGameStart = new CameraGameStart();
	CameraManager::GetInstance().CreateCamera(pCameraGameStart, "GameStartCamera");
	CameraManager::GetInstance().SetSceneCamera("GameStartCamera");
	m_pGameCamera = static_cast<CameraGame*>(CameraManager::GetInstance().GetCamera("GameCamera"));

	//=====<ステージの作成>=====
	m_pStage = new Stage00();
	m_pStage->Stage_Init();

	//=====<キャラクターの生成>=====
	Character::InitPlayerBit();
	
	//=====<選んだキャラクターの生成>=====
	Character* FirstCharacter = CreateCharacter(SceneSelect::GetFirstPlayerCharacter());
	PlayerController* pController = SceneSelect::GetFirstPlayerController();
	pController = pController == nullptr ? 
		&PlayerController::GetPlayerControllers()[0] : pController;
	FirstCharacter->SetCharacterController(pController);
	FirstCharacter->Character_Init();

	Character* SecondCharacter = CreateCharacter(SceneSelect::GetSecondPlayerCharacter());
	SecondCharacter->Character_Init();
	pController = SceneSelect::GetSecondPlayerController();
	pController = pController == nullptr ? 
		&PlayerController::GetPlayerControllers()[0] : pController;
	SecondCharacter->SetCharacterController(pController);
	
	//=====<各オブジェクトに設定するオブジェクトがあるならここでする>=====
	for (Character* pCharacter : m_Characters)
	{
		pCharacter->SetStage(m_pStage);
	}

	m_pGameCamera->SetCharacter(&m_Characters);
	m_pGameCamera->SetStage(m_pStage);
	pCameraGameStart->SetCharacter(m_Characters);
}

void SceneGame::Uninit()
{
	//=====<キャラクターの削除>=====
	for (auto it = m_Characters.begin(); it != m_Characters.end();)
	{
		(*it)->Character_Uninit();
		delete (*it);
		it = m_Characters.erase(it);
	}

	//=====<ステージの削除>=====
	m_pStage->Stage_Uninit();
	delete m_pStage;

	//=====<カメラの削除>=====
	CameraManager::GetInstance().DestroyCamera("DebugCamera", true);
}

void SceneGame::Update()
{
	switch (m_GameState)
	{
	case SceneGame::GAMESTATE::GAMESTART:
		UpdateGameStart();
		break;
	case SceneGame::GAMESTATE::GAMEPLAY:
		UpdateGamePlay();
		break;
	case SceneGame::GAMESTATE::GAMEEND:
		UpdateGameEnd();
		break;
	}
}

void SceneGame::Draw()
{
	switch (m_GameState)
	{
	case SceneGame::GAMESTATE::GAMESTART:
		DrawGameStart();
		break;
	case SceneGame::GAMESTATE::GAMEPLAY:
		DrawGamePlay();
		break;
	case SceneGame::GAMESTATE::GAMEEND:
		DrawGameEnd();
		break;
	}
}

std::vector<Character*>& SceneGame::GetCharacter()
{
	return m_Characters;
}

Stage* SceneGame::GetStage()
{
	return m_pStage;
}

Character* SceneGame::CreateCharacter(int Num)
{
	return CreateCharacter(static_cast<SelectCharacterList::CHARACTER>(Num));
}

Character* SceneGame::CreateCharacter(SelectCharacterList::CHARACTER CharacterNum)
{
	switch (CharacterNum)
	{
	case SelectCharacterList::CHARACTER::UNITYCHAN:
		m_Characters.push_back(new Character_00());
		break;
	case SelectCharacterList::CHARACTER::MAX:
		break;
	default:
		break;
	}

	auto it = m_Characters.end();
	it--;
	return (*it);
}

void SceneGame::UpdateGameStart()
{
	m_GameStartFrameCount++;

	if (m_GameStartFrameCount == 1 * 60)
	{
		m_GameStartCountUI.SetNumDraw(2);
	}

	if (m_GameStartFrameCount == 2 * 60)
	{
		m_GameStartCountUI.SetNumDraw(1);
	}

	if (m_GameStartFrameCount == 2 * 60)
	{
		m_GameStartCountUI.SetNumDraw(1);
	}

	if (m_GameStartFrameCount == 3 * 60)
	{
		m_GameStartCountUI.SetGoDraw(true);
	}
}

void SceneGame::UpdateGamePlay()
{
	//=====<キャラクターのアップデート>=====
	//ここで攻撃や移動などのアップデートを行う
	for (std::vector<Character*>::iterator it = m_Characters.begin();
		it != m_Characters.end(); it++)
	{
		(*it)->Character_Update();				//キャラクターのアップデートを行う
		(*it)->Character_ColliderInit();		//コライダーの情報を初期化してやる
												//(当たり判定を行っていないので当たっていないことにする処理)
	}

	//=====<キャラクター同士の当たり判定>=====
	Collision_Player_Player();

	//=====<キャラクターの攻撃の当たり判定>=====
	Collision_Attack_Player();

	//=====<キャラクターとステージの当たり判定>=====
	Collision_Player_Stage();
}


void SceneGame::UpdateGameEnd()
{

}

void SceneGame::Collision_Player_Player()
{
	//=====<キャラクター同士の当たり判定>=====

	//一人目のキャラクターを選択
	for (std::vector<Character*>::iterator it_first = m_Characters.begin();
		it_first != m_Characters.end(); it_first++)
	{
		//一人目のキャラクターがゲームオーバーの場合
		if ((*it_first)->GetState() == Character::STATE::GAMEOVER)
		{
			continue;
		}

		//二人目のキャラクターを選択(一人目の次のキャラクター)
		std::vector<Character*>::iterator it_second = it_first + 1;
		for (; it_second != m_Characters.end(); it_second++)
		{
			//二人目のキャラクターがゲームオーバーの場合
			if ((*it_second)->GetState() == Character::STATE::GAMEOVER)
			{
				continue;
			}

			//四角コライダーの取得
			BoxCollider* pFirstCollider = (*it_first)->GetCharacterCollider();
			BoxCollider* pSecondCollider = (*it_second)->GetCharacterCollider();

			//四角同士の当たり判定を行う
			if (!pFirstCollider->CollisionBox(*pSecondCollider))
			{
				//当たっていない場合
				continue;
			}

			//めり込んだ半分の位置で移動してやる(Xのみ)
			float NowHarfDistanceX = (pFirstCollider->GetPos().x - pSecondCollider->GetPos().x) * 0.5f;
			float CenterPosX = pSecondCollider->GetPos().x + NowHarfDistanceX;
			float FirstDirect = NowHarfDistanceX < 0.0f ? -1.0f : 1.0f;	//pFirstColliderに向かう向き
			//上の値がマイナスなら位置関係は
			//pFirstCollider ・ pSecondColliderとなる
			//上の値がプラスなら位置関係は
			//pSecondCollider ・ pFirstColliderとなる

			//一人目のキャラクターの位置設定
			(*it_first)->SetPos(
				CVector3(
					CenterPosX + ((pFirstCollider->GetSize().x * 0.5f) * FirstDirect),
					(*it_first)->GetPos().y,
					(*it_first)->GetPos().z
				)
			);

			//二人目のキャラクターの位置設定
			(*it_second)->SetPos(
				CVector3(
					CenterPosX + ((pSecondCollider->GetSize().x * 0.5f) * -FirstDirect),
					(*it_second)->GetPos().y,
					(*it_second)->GetPos().z
				)
			);
		}
	}
}

void SceneGame::DrawGameStart()
{
	//=====<ステージの描画>=====
	m_pStage->Stage_Draw();

	//=====<キャラクターの描画>=====
	for (Character* copy : m_Characters)
	{
		copy->Character_Draw();
	}

	//=====<UIの描画>=====
	EnableDepth(false);

	for (Character* copy : m_Characters)
	{
		copy->Character_UIDraw();
	}

	m_GameStartCountUI.Draw();

	EnableDepth(true);
}

void SceneGame::DrawGamePlay()
{
	//=====<ステージの描画>=====
	m_pStage->Stage_Draw();

	//=====<キャラクターの描画>=====
	for (Character* copy : m_Characters)
	{
		copy->Character_Draw();
	}

	//=====<当たり判定の描画>=====
	if (IsKeyTrigger(VK_RETURN))
	{
		ColliderDraw = !ColliderDraw;
	}

	if (ColliderDraw)
	{
		m_pStage->StageColliderDraw();
		for (Character* CharacterCopy : m_Characters)
		{
			CharacterCopy->DrawCollider();

			if (CharacterCopy->GetState() != Character::STATE::ATTACK)
			{
				continue;
			}

			std::vector<Character::ATTACKPARAM>& attackVector
				= CharacterCopy->GetAttackCollider();

			for (Character::ATTACKPARAM& AttackCopy : attackVector)
			{
				if (!AttackCopy.m_Use)
				{
					continue;
				}
				AttackCopy.m_BoxCollider.DrawCollider();
			}

		}
	}

	//=====<UIの描画>=====
	EnableDepth(false);

	for (Character* copy : m_Characters)
	{
		copy->Character_UIDraw();
	}

	EnableDepth(true);
}

void SceneGame::DrawGameEnd()
{
	
}

void SceneGame::Collision_Player_Stage()
{
	//=====<キャラクターとステージの当たり判定>=====
	std::vector<BoxCollider>* pStageCollider = m_pStage->GetStageCollider();

	for (std::vector<Character*>::iterator it_Character = m_Characters.begin();
		it_Character != m_Characters.end(); it_Character++)
	{
		//キャラクターがゲームオーバーしていたら当たり判定を行わない
		if ((*it_Character)->GetState() == Character::STATE::GAMEOVER)
		{
			continue;
		}

		BoxCollider* pCharacterCollider = (*it_Character)->GetCharacterCollider();

		//Xの移動だけの当たり判定
		for (std::vector<BoxCollider>::iterator it_StageCollider = pStageCollider->begin();
			it_StageCollider != pStageCollider->end(); it_StageCollider++)
		{
			CVector3 DiffPos = (*it_Character)->GetPos() - (*it_Character)->GetOldPos();		//前の位置から今の位置まで移動したベクトル
			CVector3 HitSize = (pCharacterCollider->GetSize() + (*it_StageCollider).GetSize()) * 0.5f;
			float NowDistanceX = pCharacterCollider->GetPos().x - (*it_StageCollider).GetPos().x;
			float AbsNowDistanceX = fabsf(NowDistanceX);
			float OldDistanceY = (pCharacterCollider->GetPos().y - DiffPos.y) - (*it_StageCollider).GetPos().y;
			float AbsOldDistanceY = fabsf(OldDistanceY);
			float OldDistanceZ = (pCharacterCollider->GetPos().z - DiffPos.z) - (*it_StageCollider).GetPos().z;
			float AbsOldDistanceZ = fabsf(OldDistanceZ);

			//Xの移動だけして当たっていたら
			if (AbsNowDistanceX < HitSize.x &&
				AbsOldDistanceY < HitSize.y &&
				AbsOldDistanceZ < HitSize.z)
			{
				CVector3 newPos = (*it_Character)->GetPos();
				float MoveDist = DiffPos.x < 0.0f ? HitSize.x : -HitSize.x;
				newPos.x = (*it_StageCollider).GetPos().x + MoveDist;
				(*it_Character)->SetPos(newPos);

				(*it_Character)->Character_HitWall();
			}
		}

		//Yの移動だけの当たり判定
		for (std::vector<BoxCollider>::iterator it_Stage = pStageCollider->begin();
			it_Stage != pStageCollider->end(); it_Stage++)
		{
			CVector3 DiffPos = (*it_Character)->GetPos() - (*it_Character)->GetOldPos();		//前の位置から今の位置まで移動したベクトル
			CVector3 HitSize = (pCharacterCollider->GetSize() + (*it_Stage).GetSize()) * 0.5f;
			float NowDistanceX = pCharacterCollider->GetPos().x - (*it_Stage).GetPos().x;
			float AbsNowDistanceX = fabsf(NowDistanceX);
			float NowDistanceY = pCharacterCollider->GetPos().y - (*it_Stage).GetPos().y;
			float AbsNowDistanceY = fabsf(NowDistanceY);
			float OldDistanceZ = (pCharacterCollider->GetPos().z - DiffPos.z) - (*it_Stage).GetPos().z;
			float AbsOldDistanceZ = fabsf(OldDistanceZ);

			//Yの移動だけして当たっていたら
			if (AbsNowDistanceX < HitSize.x &&
				AbsNowDistanceY < HitSize.y &&
				AbsOldDistanceZ < HitSize.z)
			{
				CVector3 newPos = (*it_Character)->GetPos();
				float MoveDist = DiffPos.y < 0.0f ?
					HitSize.y - (pCharacterCollider->GetType() == BoxCollider::BOXTYPE::FOOT ? (pCharacterCollider->GetSize().y * 0.5f) : 0.0f) :	//上からあたった
					-HitSize.y;	//下からあたった
				newPos.y = (*it_Stage).GetPos().y + MoveDist;
				(*it_Character)->SetPos(newPos);

				//上から移動したか下から移動したかで判定
				if (DiffPos.y < 0.0f)
				{
					(*it_Character)->Character_HitGround();
				}
				else
				{
					(*it_Character)->Character_HitCeiling();
				}
			}
		}

		//Zの移動だけの当たり判定
		for (std::vector<BoxCollider>::iterator it_Stage = pStageCollider->begin();
			it_Stage != pStageCollider->end(); it_Stage++)
		{

			CVector3 DiffPos = (*it_Character)->GetPos() - (*it_Character)->GetOldPos();		//前の位置から今の位置まで移動したベクトル
			CVector3 HitSize = (pCharacterCollider->GetSize() + (*it_Stage).GetSize()) * 0.5f;
			float NowDistanceX = pCharacterCollider->GetPos().x - (*it_Stage).GetPos().x;
			float AbsNowDistanceX = fabsf(NowDistanceX);
			float NowDistanceY = pCharacterCollider->GetPos().y - (*it_Stage).GetPos().y;
			float AbsNowDistanceY = fabsf(NowDistanceY);
			float NowDistanceZ = pCharacterCollider->GetPos().z - DiffPos.z - (*it_Stage).GetPos().z;
			float AbsNowDistanceZ = fabsf(NowDistanceZ);

			//Zの移動だけして当たっていたら
			if (AbsNowDistanceX < HitSize.x &&
				AbsNowDistanceY < HitSize.y &&
				AbsNowDistanceZ < HitSize.z)
			{
				CVector3 newPos = (*it_Character)->GetPos();
				float MoveDist = DiffPos.z < 0.0f ? HitSize.z : -HitSize.z;
				newPos.z = (*it_Stage).GetPos().z + MoveDist;
				(*it_Character)->SetPos(newPos);
			}
		}
	}
}

void SceneGame::Collision_Attack_Player()
{
	//=====<キャラクターの攻撃の当たり判定>=====
	//上で設定した攻撃の当たり判定を使って
	//キャラクター同士の攻撃の当たり判定を行う
	for (Character* AttackCharacter : m_Characters)
	{
		//キャラクターがゲームオーバーしていたら当たり判定を行わない
		if (AttackCharacter->GetState() == Character::STATE::GAMEOVER)
		{
			continue;
		}

		//キャラクターが攻撃していなければ次のキャラクターに
		if (AttackCharacter->GetState() != Character::STATE::ATTACK)
		{
			continue;
		}

		//攻撃しているキャラクターから攻撃の当たり判定を取ってくる
		std::vector<Character::ATTACKPARAM>& attackParamVector = AttackCharacter->GetAttackCollider();

		//配列の頭から攻撃を見ていく
		for (Character::ATTACKPARAM& Character_Attack : attackParamVector)
		{
			//前のフレームに依存しないビットはここで0にしておく
			Character_Attack.m_HitCharacterBit = 0x00;
			Character_Attack.m_HitTriggerCharacterBit = 0x00;

			if (!Character_Attack.m_Use)
			{
				continue;
			}

			//攻撃を受けるキャラクター
			for (Character* HitCharacter : m_Characters)
			{
				//受けるキャラクターがゲームオーバーの場合
				if (HitCharacter->GetState() == Character::STATE::GAMEOVER)
				{
					continue;
				}

				//攻撃しているキャラクターと受けるキャラクターが同じ場合
				if (AttackCharacter == HitCharacter)
				{
					continue;
				}

				//当てるキャラクターが無敵の場合当たり判定を行わない
				if (HitCharacter->IsInvincible())
				{
					continue;
				}

				//攻撃と受けるキャラクターの当たり判定
				if (Character_Attack.m_BoxCollider.CollisionBox(*HitCharacter->GetCharacterCollider()))
				{
					//当たったキャラクターの情報を入れる
					Character_Attack.m_HitCharacterBit |= HitCharacter->GetCharacterBit();
					Character_Attack.m_HitTriggerCharacterBit = ~Character_Attack.m_haveHitCharacterBit & HitCharacter->GetCharacterBit();
					Character_Attack.m_haveHitCharacterBit |= HitCharacter->GetCharacterBit();

					//攻撃を当てるの判定
					if ((HitCharacter->GetCharacterBit() & Character_Attack.m_CanAttackCharacterBit) != 0x00)
					{
						//====<当たった時の処理>====
						CVector3 HitCharacterPos = HitCharacter->GetPos();
						HitCharacterPos.y += 0.01f;			//少し浮かさないと下で地面に当たってしまう
						HitCharacter->SetPos(HitCharacterPos);

						//当たった時の処理をする
						switch (AttackCharacter->GetAttack())
						{
						case Character::ATTACK::ATTACK_11:
							AttackCharacter->Attack11_Hit(HitCharacter);
							break;
						case Character::ATTACK::ATTACK_12:
							AttackCharacter->Attack12_Hit(HitCharacter);
							break;
						case Character::ATTACK::ATTACK_13:
							AttackCharacter->Attack13_Hit(HitCharacter);
							break;
						case Character::ATTACK::ATTACK_S2:
							AttackCharacter->AttackS2_Hit(HitCharacter);
							break;
						case Character::ATTACK::ATTACK_S4:
							AttackCharacter->AttackS4_Hit(HitCharacter);
							break;
						case Character::ATTACK::ATTACK_AIRN:
							AttackCharacter->AttackAirN_Hit(HitCharacter);
							break;
						case Character::ATTACK::SPECIAL_N:
							AttackCharacter->SpecialN_Hit(HitCharacter);
							break;
						case Character::ATTACK::SPECIAL_AIRN:
							AttackCharacter->SpecialAirN_Hit(HitCharacter);
							break;
						case Character::ATTACK::MAX:
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

