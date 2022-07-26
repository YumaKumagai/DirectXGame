#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度ベクトル</param>
	void Initialize(Model* model, const Vector3& position,
		const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

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
	// デスフラグを取得
	bool IsDead()const { return isDead_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 衝突半径
	float radius_ = 1.f;
	// モデルのポインタ
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// デスタイマー
	int32_t deathTimer = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

};
