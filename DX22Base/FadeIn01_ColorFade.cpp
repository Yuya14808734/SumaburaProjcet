#include "FadeIn01_ColorFade.h"

FadeInColorFade::FadeInColorFade()
{
	//=====<カウントの初期化>=====
	m_CountTime = 0.0f;

	//=====<テクスチャを読み込む>=====
	SetTexture("Assets/Texture/WhiteTexture.png");

	m_color.w = 0.0f;
	m_FadeTime = 1.0f;
}

FadeInColorFade::~FadeInColorFade()
{
}

void FadeInColorFade::FadeUpdate()
{
	m_CountTime += 1.0f / 60.0f;

	if (m_CountTime > m_FadeTime)
	{
		m_FadeEnd = true;
		m_CountTime = m_FadeTime;
	}

	m_color.w = 1.0f - (m_CountTime / m_FadeTime);
}

void FadeInColorFade::SetFadeTime(float time)
{
	m_FadeTime = time;
}