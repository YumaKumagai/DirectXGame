#pragma once
#include "Model.h"

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

	// Phase::Approach,default
	void UpdateApproach();

	// Phase::Leave
	void UpdateLeave();

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

};

