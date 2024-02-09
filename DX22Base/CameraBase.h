//#pragma once
#ifndef CAMERABASE
#define CAMERABASE
#include <DirectXMath.h>
#include "Calculation.h"
#include "Collider.h"

class CameraBase 
{
public :
	CameraBase();
	virtual ~CameraBase();
	virtual void Init();
	virtual void Uninit();
	virtual void Update() = 0;
	virtual void ChangeInit();
	virtual void ChangeUninit();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	void SetPos(const CVector3& pos);
	void SetPos(const DirectX::XMFLOAT3& pos);
	void SetLookPos(const CVector3& pos);
	void SetLookPos(const DirectX::XMFLOAT3& pos);
	const CVector3& GetPos();
	const CVector3& GetLookPos();
	DirectX::XMFLOAT3 GetPos_xmfloat();
	DirectX::XMFLOAT3 GetLook_xmfloat();
	float GetFovAngle();
	float GetAspect();
	float GetNearLength();
	float GetFarLength();


	CVector3 GetRightVector();
	CVector3 GetUpVector();

	void CreateViewingFrustum();
	bool CheckInObject(const CVector3& Point, float Radius);

	CVector2 ChangeScreenPos(const CVector3& Pos3D);
	void SetOrtho(bool ortho);							//平行投影

protected:
	CVector3 m_pos;
	CVector3 m_look;
	CVector3 m_up;					//ビュー行列の設定に必要な変数

	PlaneCollider m_Planes[6];		//視野錐台で使う面

	//============================================================
	// 透視投影を行う場合の変数
	//============================================================
	float m_fovy	= 0.0f;					//カメラ縦方向の画角
	float m_aspect	= 0.0f;					//縦を1とした横の比率
	
	//============================================================
	// 並行投影を行う場合の変数
	//============================================================
	bool m_isOrtho		= false;		//平行投影を行うか
	float m_ViewWidth	= 0.0f;
	float m_ViewHeight	= 0.0f; 
	
	//============================================================
	// 共通で使う変数
	//============================================================
	float m_near = 0.0f;					//どこから映すか
	float m_far = 0.0f;					//どこまで映すか
};


#endif //CAMERABASE