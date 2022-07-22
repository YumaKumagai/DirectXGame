#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy01.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 初期座標設定
	worldTransform_.translation_ = { 0,2,50 };

	// 接近フェーズに設定
	phase_ = Phase::Approach;

	// 接近フェーズ初期化
	InitializeApproach();

}

void Enemy::Update()
{
	// 弾削除,更新
	{
		// デスフラグの立った弾を削除
		bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
			{
				return bullet->IsDead();
			});

		// 弾の更新処理
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
		{
			bullet->Update();
		}

	}

	// フェーズ別更新処理
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

void Enemy::InitializeApproach()
{
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;

}

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

	// 弾発射
	Fire();

}

void Enemy::UpdateLeave()
{
	// 移動速度
	const Vector3 leaveVelocity(-0.1f, 0.1f, -0.1f);

	// 移動
	worldTransform_.translation_ += leaveVelocity;
}

void Enemy::Fire()
{
	// 発射タイマーカウントダウン
	if (fireTimer_ > 0)
	{
		fireTimer_--;
	}
	// 指定時間に達した
	else
	{
		// 弾発射
		{
			// 弾の速度
			const float kBulletSpeed = -1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newBullet =
				std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_,
				velocity);

			// 弾を登録する
			bullets_.push_back(std::move(newBullet));
		}

		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;

	}

}

#pragma endregion

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画処理
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}
