#include "SceneGame_EndState.h"
#include "Scene.h"
#include "Scene01_Select.h"

void SceneGame_EndState::Init()
{
	//ゲームエンドと描画する
	m_pGameEndTextUI->StartAnimetion();
	m_pGameEndTextUI->SetStartSize(CVector2(640.0f, 304.0f) * 2.0f);
	m_pGameEndTextUI->SetEndSize(CVector2(640.0f, 304.0f));

	m_SceneChangeCount = 0;
}

void SceneGame_EndState::Uninit()
{
}

void SceneGame_EndState::Update()
{
	m_pGameEndTextUI->Update();

	m_SceneChangeCount++;

	if (m_SceneChangeCount > 60 * 3)
	{
		//シーンの切り替え
		CScene::SetScene<SceneSelect>();
	}
}

void SceneGame_EndState::Draw()
{
	//=====<ステージの描画>=====
	m_pStage->Stage_Draw();

	//=====<キャラクターの描画>=====
	for (Character* copy : (*m_pCharacters))
	{
		copy->Character_Draw();
	}

	//=====<UIの描画>=====
	EnableDepth(false);

	for (Character* copy : (*m_pCharacters))
	{
		copy->Character_UIDraw();
	}

	m_pGameEndTextUI->Draw();

	EnableDepth(true);
}
