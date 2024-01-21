//#pragma once
#ifndef UI_BASE_H
#define UI_BASE_H

#include "DirectX.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DirectXTex/Texture.h"
#include "Calculation.h"
#include "Shader.h"


class Image2D
{
public:
	Image2D();
	virtual ~Image2D();
	virtual void Update();
	void Draw();
protected:
	virtual void PrevDraw();

public:
	void SetTexture(const char* FilePath);
	void SetTexture(ID3D11ShaderResourceView* pTextureResource);
	void ReleaseTexture();
	void SetPos(const DirectX::XMFLOAT3& pos);
	void SetPos(const CVector3& pos);
	void SetPos(const CVector2& pos);
	const CVector3& GetPos();
	void SetSize(const DirectX::XMFLOAT2& size);
	void SetSize(const CVector2& size);
	const CVector2& GetSize();
	void SetUVPos(const DirectX::XMFLOAT2& uvpos);
	void SetUVPos(const CVector2& uvpos);
	const CVector2& GetUVPos();
	void SetUVSize(const DirectX::XMFLOAT2& uvsize);
	void SetUVSize(const CVector2& uvsize);
	const CVector2& GetUVSize();
	void SetVertexShader(VertexShader* pVS,bool MemoryDelete = false);
	void SetPixelShader(PixelShader* pPS, bool MemoryDelete = false);

private:
	ID3D11ShaderResourceView* m_pTexture = nullptr;
	bool m_isPathLoad = false;
	VertexShader* m_pVertexShader = nullptr;
	PixelShader* m_pPixelShader = nullptr;
public:
	CVector3 m_BasePos;				//位置の原点となる座標
	CVector3 m_pos;					//位置
	CVector2 m_size;				//サイズ
	CVector2 m_UVpos;				//UV位置
	CVector2 m_UVsize;				//UVサイズ
	DirectX::XMFLOAT4 m_color;		//色
	bool m_IsVisible = true;
};
#endif