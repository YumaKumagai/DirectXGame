#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/enemy01.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 初期座標設定
	worldTransform_.translation_ = { 0,2,50 };

	// 接近フェーズに設定
	phase_ = Phase::Approach;

}

void Enemy::Update()
{
	switch (phase_)
	{
	case Phase::Approach:
	default:

		UpdateApproach();

		break;
	case Phase::Leave:

		UpdateLeave();

		break;
	}


	// 行列更新
	{
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// スケーリング
		{
			Matrix4 matScale = MathUtility::Matrix4Scaling(
				worldTransform_.scale_.x,
				worldTransform_.scale_.y,
				worldTransform_.scale_.z
			);
			worldTransform_.matWorld_ *= matScale;
		}

		// 回転
		{
			Matrix4 matRot = MathUtility::Matrix4RotationZ(worldTransform_.rotation_.z);
			matRot *= MathUtility::Matrix4RotationX(worldTransform_.rotation_.x);
			matRot *= MathUtility::Matrix4RotationY(worldTransform_.rotation_.y);
			worldTransform_.matWorld_ *= matRot;
		}

		// 平行移動
		{
			Matrix4 matTrans = MathUtility::Matrix4Translation(
				worldTransform_.translation_.x,
				worldTransform_.translation_.y,
				worldTransform_.translation_.z
			);
			worldTransform_.matWorld_ *= matTrans;
		}

		// 行列の転送
		worldTransform_.TransferMatrix();
	}

}
#pragma region Update分割メソッド

void Enemy::UpdateApproach()
{
	// 移動速度
	const Vector3 approachVelocity(0, 0, -0.25f);

	// 移動
	worldTransform_.translation_ += approachVelocity;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave()
{
	// 移動速度
	const Vector3 leaveVelocity(-0.1f, 0.1f, -0.1f);

	// 移動
	worldTransform_.translation_ += leaveVelocity;
}

#pragma endregion

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
