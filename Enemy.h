#pragma once
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>

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

	// 発射間隔
	static const int kFireInterval = 60;

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

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

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

	// 発射タイマー
	int32_t fireTimer_ = 0;

};

