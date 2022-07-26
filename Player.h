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
#pragma region Update内メソッド

	/// <summary>
	/// Y軸回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 十字キー移動
	/// </summary>
	void Move();

	/// <summary>
	/// 自弾発射・更新処理
	/// </summary>
	void Attack();

#pragma endregion

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection_);

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition()const
	{
		return Vector3(
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2]
		);
	}
	// 衝突半径を取得
	float GetRadius()const { return radius_; }
	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 衝突半径
	float radius_ = 1.f;
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
