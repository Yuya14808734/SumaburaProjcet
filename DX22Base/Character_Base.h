#pragma once
#include "Calculation.h"
#include "Collider.h"
#include "ModelDrawer.h"
#include <vector>
#include "Character_DamageUI.h"
#include "XboxKeyboard.h"
#include "Player_Controller.h"

class Character
{
public:
	enum class STATE : int
	{
		IDLE = 0,	//待機
		WALK,		//歩く
		DASH,		//走る
		ATTACK,		//攻撃
		BLOWAWAY,	//吹っ飛ばす
		JUMPIN,		//ジャンプの入り
		JUMP,		//ジャンプ
		AIRMOVE,	//空中移動
		FALLDOWN,	//落下(空中移動とは違いくるくる落ちる感じ)
		LEANBACK,	//殴られてのけぞっている状態
		DOWN,		//倒れる
		WAKEUP,		//起き上がる
		HITSTOP,	//ヒットストップ
		MAX,
	};
	enum class ATTACK : int
	{
		ATTACK_11,		//弱1
		ATTACK_12,		//弱2
		ATTACK_13,		//弱3
		ATTACK_S2,		//横強
		ATTACK_S4,		//横スマッシュ
		ATTACK_AIRN,	//空N
		SPECIAL_N,		//必殺技普通
		SPECIAL_AIRN,	//空中必殺技
		MAX,
	};

	enum class LOOKDIR : int
	{
		RIGHT = 0,
		LEFT,
		UP,
		DOWN,
		MAX
	};

	struct ATTACKPARAM
	{
		bool m_Use = false;									//この攻撃の当たり判定を行うか
		BoxCollider m_BoxCollider;							//攻撃の当たり判定
		unsigned int m_HitCharacterBit = 0x00;				//前のフレームで当たったキャラクターのビット番号が入る
		unsigned int m_CanAttackCharacterBit = 0x00;		//当たることが出来るキャラクターのビット番号
		unsigned int m_haveHitCharacterBit = 0x00;			//今までで当たったことがあるキャラクタービット番号
		unsigned int m_HitTriggerCharacterBit = 0x00;		//前のフレームで初めて当たった事があるキャラクターのビット番号
	};

	struct MOVEPARAMETER
	{
		float	m_WalkSpeed = 0.0f;		//歩くスピード
		float	m_DashSpeed = 0.0f;		//走るスピード
		float	m_AirSideMoveSpeed = 0.0f;		//落ちているときの横移動のスピード
		float	m_Friction = 0.0f;		//摩擦量
		float	m_AirDrag = 0.0f;		//空気抵抗
	};

	struct JUMPPARAMETER
	{
		int		m_MaxJumpCount = 0;				//ジャンプできる最大数
		int		m_MiniJumpPushButtonCount = 0;	//小ジャンプするときのフレーム
		int		m_JumpChargeCount = 0;			//ジャンプするまでのチャージ時間
		float	m_FirstMiniJumpPower = 0.0f;	//小ジャンプをするときの力
		float	m_FirstJumpPower = 0.0f;		//ジャンプするときの力
		float	m_SecondJumpPower = 0.0f;		//二回目のジャンプの力
		float	m_JumpUpReduction = 0.0f;		//ジャンプして上に上がるときの重力
		float	m_FallDownGravity = 0.0f;		//ジャンプし終わって下に下がるときの重力
		float	m_DefaultFallSpeed = 0.0f;		//最大落下速度
		float	m_SpeedUpFallSpeed = 0.0f;		//落下中に下を押した場合の落下量
		float	m_ChangeFallSpeed = 0.0f;		//落下に変わるときのスピード
	};

	struct BLOWAWAYPARAMETER
	{
		float m_SmashMitigation = 0.0f;			//吹っ飛ばされた時にどれくらい弱くしていくか
		float m_VectorChangePower = 0.0f;		//吹っ飛ばされた時にベクトル変更する割合
		float m_MinimumSmashLength = 0.0f;		//吹っ飛んでいると判定する最低距離速度
	};

public:
	static void InitPlayerBit();	//下の関数でもらえる番号を初期化する
protected:
	static int GetNewPlayerBit();	//新しいプレイヤーの番号(2進数)を受け取る
									//変なところで呼ぶと番号がずれてしまうのでInit以外で呼ばないように
private:
	static int m_NewPlayerBit;		//新しいプレイヤーのビット番号


public:
	Character() {};
	~Character() {};

	//==========================================================================
	void Character_Init();								//キャラクターの初期化
	void Character_Uninit();							//キャラクターの終了処理
	void Character_Update();							//キャラクターの更新
	void Character_Draw();								//キャラクターの描画
	void Character_UIDraw();
	void StateInit(Character::STATE state);				//状態の初期化
	void StateUninit(Character::STATE state);			//状態の終了処理
	void StateUpdate(Character::STATE state);			//状態のアップデート
	//==========================================================================
	int GetCharacterBit();								//キャラクター番号の取得
	void SetState(Character::STATE state);				//キャラクターの状態変更 外部が呼ぶ関数
	void SetHitStop(int HitStopCount,Character::STATE NextState);	//ヒットストップを設定	 外部が呼ぶ関数
	const Character::STATE& GetState() const;			//今の状態の取得
	const Character::ATTACK& GetAttack() const;			//今している攻撃情報
	ModelDrawer* GetModel() const;						//モデル情報の取得

	//==========================================================================
	const CVector3& GetPos() const;						//位置の取得
	void SetPos(const CVector3& pos);					//位置の設定
	const CVector3& GetOldPos() const;					//前の座標を取得
	void SetOldPos();									//前の座標を設定
	const CVector3& GetScale() const;					//サイズの取得
	void SetScale(const CVector3& scale);				//サイズの設定
	const CQuaternion& GetRotate() const;				//回転量の取得
	void SetRotate(const CQuaternion& rotate);			//回転量の設定(Quaternion)
	void SetRotate(const CVector3& rotate);				//回転量の設定(度数法)
	void SetShake(bool shake);							//キャラクターを揺らすか							
	bool GetShake();									//キャラクターを揺らす	
	void SetLookRight();								//右を向かせる
	void SetLookLeft();									//左を向かせる
	//==========================================================================
	void AddForce(const CVector3& force);				//力を足してやる
	void SetForce(const CVector3& force);				//力を設定
	float GetDamage() const;							//ダメージの取得
	void AddDamage(float damage);						//ダメージの加算
	void SetDamage(float damage);						//ダメージの設定
	//==========================================================================
	BoxCollider* GetCharacterCollider() const;			//キャラクター、ステージ当たり判定の取得
	std::vector<ATTACKPARAM>& GetAttackCollider();		//攻撃コライダーの取得
	void Character_ColliderInit();
	void Character_HitCeiling();						//天井に当たった	
	void Character_HitGround();							//地面に当たった
	void Character_HitWall();							//壁に当たった
	void DrawCollider();								//コライダーの描画
	//==========================================================================
	void SetCharacterController(PlayerController pController);


protected:
	void ChangeAttack(Character::ATTACK attack);		//攻撃を設定する関数 内部で呼ぶ関数
	void ChangeState(Character::STATE state);			//状態を変更する関数 内部で呼ぶ関数
	void ChangeHitStop(int HitStopCount, Character::STATE NextState);				//状態を変更する関数 内部で呼ぶ関数

protected:

	virtual void Init() {};			//継承先の初期化
	virtual void Uninit() {};		//継承先の終了処理
	virtual void Update() {};		//継承先の更新
	virtual void Draw() {};			//継承先の描画

	//==========================================================================
	virtual void IdleInit();		//止まっているときの初期化
	virtual void IdleUninit();		//止まっているときの終了処理
	virtual void IdleUpdate();		//止まっているときのアップデート
	//==========================================================================
	virtual void WalkInit();		//歩く時の初期化
	virtual void WalkUninit();		//歩く時の終了処理
	virtual void WalkUpdate();		//歩く時のアップデート
	//==========================================================================
	virtual void DashInit();		//走るときの初期化
	virtual void DashUninit();		//走るときの終了処理
	virtual void DashUpdate();		//走るときのアップデート
	//==========================================================================
	virtual void AttackInit();		//攻撃するときの初期化
	virtual void AttackUninit();	//攻撃するときの終了処理
	virtual void AttackUpdate();	//攻撃するときのアップデート
	//==========================================================================
	virtual void BlowAwayInit();		//吹っ飛ばされる時の初期化
	virtual void BlowAwayUninit();		//吹っ飛ばしの終了処理
	virtual void BlowAwayUpdate();		//吹っ飛ばしのアップデート
	//==========================================================================
	virtual void JumpInInit();		//ジャンプ始めの初期化
	virtual void JumpInUpdate();	//ジャンプ始めのアップデート
	virtual void JumpInUninit();	//ジャンプ始めの終了処理
	//==========================================================================
	virtual void JumpInit();		//ジャンプしたときの初期化
	virtual void JumpUninit();		//ジャンプしたときの終了処理
	virtual void JumpUpdate();		//ジャンプしたときのアップデート
	//==========================================================================
	virtual void AirMoveInit();		//空中にいるときの初期化
	virtual void AirMoveUninit();	//空中にいるときの終了処理
	virtual void AirMoveUpdate();	//落ちているときのアップデート
	//==========================================================================
	virtual void FallDownInit();
	virtual void FallDownUninit();
	virtual void FallDownUpdate();
	//==========================================================================
	virtual void DownInit();		//倒れた時の初期化
	virtual void DownUninit();		//倒れた時の終了処理
	virtual void DownUpdate();		//倒れている状態のアップデート
	//==========================================================================
	virtual void WakeUpInit();
	virtual void WakeUpUninit();
	virtual void WakeUpUpdate();
	//==========================================================================
	virtual void LeanBackInit();
	virtual void LeanBackUninit();
	virtual void LeanBackUpdate();
	//==========================================================================
	virtual void HitStopInit();		//ヒットストップの初期化
	virtual void HitStopUninit();	//ヒットストップの終了処理
	virtual void HitStopUpdate();	//ヒットストップ状態のアップデート
	
	//==========================================================================
	//スマブラと同じような名前にしています
	//https://www.youtube.com/watch?v=V40sMUAE5ek
	//==========================================================================
	virtual void Attack11_Init() {};		//弱1
	virtual void Attack11_Update() {};		//弱1
	virtual void Attack11_Uninit() {};		//弱1
	//==========================================================================
	virtual void Attack12_Init() {};		//弱2
	virtual void Attack12_Update() {};		//弱2
	virtual void Attack12_Uninit() {};		//弱2
	//==========================================================================
	virtual void Attack13_Init() {};		//弱3
	virtual void Attack13_Update() {};		//弱3
	virtual void Attack13_Uninit() {};		//弱3
	//==========================================================================
	virtual void AttackS2_Init() {};		//横強
	virtual void AttackS2_Update() {};		//横強
	virtual void AttackS2_Uninit() {};		//横強
	//==========================================================================
	virtual void AttackS4_Init() {};		//横スマッシュ
	virtual void AttackS4_Update() {};		//横スマッシュ
	virtual void AttackS4_Uninit() {};		//横スマッシュ
	//==========================================================================
	virtual void AttackAirN_Init() {};		//空N
	virtual void AttackAirN_Update() {};	//空N
	virtual void AttackAirN_Uninit() {};	//空N
	//==========================================================================
	virtual void SpecialN_Init() {};		//通常必殺技
	virtual void SpecialN_Update() {};		//通常必殺技
	virtual void SpecialN_Uninit() {};		//通常必殺技
	//==========================================================================
	virtual void SpecialAirN_Init() {};		//通常必殺技(空中)
	virtual void SpecialAirN_Update() {};	//通常必殺技(空中)
	virtual void SpecialAirN_Uninit() {};	//通常必殺技(空中)

public:
	virtual void Attack11_Hit(Character* HitCharacter) {};			//弱1
	virtual void Attack12_Hit(Character* HitCharacter) {};			//弱2
	virtual void Attack13_Hit(Character* HitCharacter) {};			//弱3
	virtual void AttackS2_Hit(Character* HitCharacter) {};			//横強
	virtual void AttackS4_Hit(Character* HitCharacter) {};			//横スマッシュ
	virtual void AttackAirN_Hit(Character* HitCharacter) {};		//空N
	virtual void SpecialN_Hit(Character* HitCharacter) {};			//通常必殺技
	virtual void SpecialAirN_Hit(Character* HitCharacter) {};		//通常必殺技(空中)


protected:

	virtual void HitCeiling() {};		//天井に当たった時に呼ぶ
	virtual void HitGround() {};	//地面に当たった時に呼ばれる
	virtual void HitWall() {};		//壁に当たった時に呼ぶ

public:
	//===============================================================================
	// パラメータ一覧
	//===============================================================================
	
	//-------------------------------------------------------------------------------
	// 横移動に関するパラメータ
	//-------------------------------------------------------------------------------

	MOVEPARAMETER m_MoveParameter;

	void SetMoveParameter(const MOVEPARAMETER& MoveParameter)
	{
		m_MoveParameter = MoveParameter;
	}

	const MOVEPARAMETER& GetMoveParameter()
	{
		return m_MoveParameter;
	}

	//-------------------------------------------------------------------------------
	// ジャンプに関するパラメータ
	//-------------------------------------------------------------------------------

	JUMPPARAMETER m_JumpParameter;

	void SetjumpParameter(const JUMPPARAMETER& JumpParameter)
	{
		m_JumpParameter	= JumpParameter;
	}

	const JUMPPARAMETER& GetJumpParameter()
	{
		return m_JumpParameter;
	}

	//-------------------------------------------------------------------------------
	// 吹っ飛ばされた時に関するパラメータ
	//-------------------------------------------------------------------------------
	
	BLOWAWAYPARAMETER m_BlowAwayParameter;

	void SetBlowAwayParameter(const BLOWAWAYPARAMETER& BlowAwayParameter)
	{
		m_BlowAwayParameter = BlowAwayParameter;
	}

	const BLOWAWAYPARAMETER& GetBlowAwayParameter()
	{
		return m_BlowAwayParameter;
	}

protected:
	//===============================================================================
	// 変数一覧
	//===============================================================================
	int		m_PlayerBit = 0x00;				//このキャラクターが何番なのかを入れる
	PlayerController m_Controller;		//キャラクターのコントローラー

	//-------------------------------------------------------------------------------
	// プレイヤーステートに関する変数
	//-------------------------------------------------------------------------------
	Character::STATE m_NowState	 = STATE::MAX;	//キャラクターの状態
	Character::STATE m_NextState = STATE::MAX;	//キャラクターの状態
	Character::ATTACK m_NowAttack = ATTACK::MAX;	//プレイヤーがしている攻撃
	Character::LOOKDIR m_NowLookDir = LOOKDIR::MAX;	//プレイヤーが見ている方向
	bool	m_ChangeState		= false;
	
	//-------------------------------------------------------------------------------
	// モデル描画に関する変数
	//-------------------------------------------------------------------------------
	ModelDrawer m_CharacterModel;			//キャラクターのモデル
	
	//-------------------------------------------------------------------------------
	// 座標に関する変数
	//-------------------------------------------------------------------------------
	CVector3 m_pos;							//座標
	CVector3 m_AddDrawPos;					//描画するときにずらす座標
	CVector3 m_oldPos;						//前の座標
	CVector3 m_scale;						//大きさ
	CQuaternion m_rotate;					//回転量
	CVector3 m_Velocity;					//重力など
	CVector3 m_MoveVector;					//コントローラーの移動量
	CVector3 m_ShiftCenterPos;				//回転や拡縮をする中心位置をずらす
		
	//-------------------------------------------------------------------------------
	// 当たり判定に関する変数
	//-------------------------------------------------------------------------------
	BoxCollider m_CharacterCollider;		//プレイヤーの当たり判定
	std::vector<ATTACKPARAM> m_AttackCollider;	//攻撃したときの当たり判定
	float m_DamagePercentage = 0.0f;		//ダメージの量

	//-------------------------------------------------------------------------------
	// ジャンプに関する変数
	//-------------------------------------------------------------------------------
	int m_JumpCount = 0;					//今ジャンプした回数
	int m_JumpCharageCount = 0;				//ジャンプをチャージする時のカウント

	//-------------------------------------------------------------------------------
	//ステージに当たった判定に使う変数
	//-------------------------------------------------------------------------------
	bool m_HitGround = false;				//前のフレームで地面に当たったか
	bool m_HitCeiling = false;				//前のフレームで天井に当たったか
	bool m_HitWall = false;					//前のフレームで壁に当たったか

	//-------------------------------------------------------------------------------
	//ステージに当たった判定に使う変数
	//-------------------------------------------------------------------------------
	Character_DamageUI m_DamageUI;

	//-------------------------------------------------------------------------------
	// ヒットストップに関する変数
	//-------------------------------------------------------------------------------
	bool m_Shake = false;					//プレイヤーを揺らすか
	unsigned int m_HitStopCount = 0;		//ヒットストップする時間
	Character::STATE m_HitStopNextState;	//ヒットストップ後の状態

};