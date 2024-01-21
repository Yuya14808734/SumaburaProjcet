#pragma once
#include "Scene.h"
#include "Image2D.h"
#include "Image3D.h"
#include "StatePattern.h"

class SceneTitle : public CScene
{
	enum class TITLESTATE : int
	{
		TITLEDRAW = 0,
		TITLESELECT,
		MAX,
	};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	State* SetNextState(TITLESTATE TitleState);		//�X�e�[�g�̐ݒ�
	void ChangeNextState();						//�X�e�[�g�̎擾

private:
	StateContext m_TitleStateContext;
	Image2D m_BackGround;
};