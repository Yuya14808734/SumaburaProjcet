#include "Character_00.h"

void Character_00::AttackS2_Init()
{
	//当たり判定の作成
	BoxCollider AttackBox;
	AttackBox.CreateBox(BoxCollider::BOXTYPE::FOOT, m_pos , CVector3(2.0f, 2.0f, 2.0f));
}

void Character_00::AttackS2_Update()
{

}

void Character_00::AttackS2_Uninit()
{

}