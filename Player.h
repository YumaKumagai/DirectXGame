#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "PlayerBullet.h"
#include <memory>

#define TO_RADIAN(deg) (deg*M_PI/180)

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
#pragma region Update分割メソッド

	/// <summary>
	/// Y軸回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 十字キー移動
	/// </summary>
	void Move();

	/// <summary>
	/// 座標をデバッグテキストで表示
	/// </summary>
	void DisplayCoord(float posX, float posY)const;

	/// <summary>
	/// 自弾発射・更新処理
	/// </summary>
	void Attack();

#pragma endregion

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection_);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// シングルトンインスタンス用ポインタ
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	// 弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};
