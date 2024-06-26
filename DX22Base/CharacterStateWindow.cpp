#include "CharacterStateWindow.h"

void CharacterStateWindow::Init()
{
	m_WindowName = "AnimetionInspector";

	m_Pos = { 950.0f,50.0f };
	m_Size = { 300.0f,200.0f };
}

void CharacterStateWindow::Uninit()
{

}

void CharacterStateWindow::DrawUpdate()
{
	ImGui::Text("Models");

	Character::JUMPPARAMETER JumpParameter = m_pCharacter->GetJumpParameter();

	ImGui::InputFloat(u8"ジャンプ力", &JumpParameter.FirstJumpPower);
	ImGui::InputFloat(u8"ジャンプ重力", &JumpParameter.JumpUpReduction);
	ImGui::InputFloat(u8"下に下がるときの重力", &JumpParameter.FallDownGravity);
	ImGui::InputFloat(u8"最大落下速度", &JumpParameter.DefaultFallSpeed);
	ImGui::InputFloat(u8"落下に変わるスピード", &JumpParameter.ChangeFallSpeed);

	m_pCharacter->SetjumpParameter(JumpParameter);
}

void CharacterStateWindow::SetCharacter(Character* pCharacter)
{
	m_pCharacter = pCharacter;
}
