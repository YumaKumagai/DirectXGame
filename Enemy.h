#pragma once
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include "Player.h"

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
#pragma region Update分割メソッド

	// 接近フェーズ初期化処理
	void InitializeApproach();

	// 接近フェーズ更新処理
	void UpdateApproach();

	// 離脱フェーズ更新処理
	void UpdateLeave();

	// 弾発射
	void Fire();

#pragma endregion

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	// Player登録
	static void SetPlayer(const Player* player);

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
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 衝突判定
	float radius_ = 1.f;

	// モデルのポインタ
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 行動フェーズ
	enum class Phase
	{
		Init,		// 初期フェーズ
		Approach,	// 接近
		Leave,		// 離脱
	} phase_ = Phase::Init;

	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// 発射間隔
	static const int kFireInterval = 60;

	// 発射タイマー
	int32_t fireTimer_ = 0;

	// player参照用ポインタ
	static const Player* kPlayer;

};

