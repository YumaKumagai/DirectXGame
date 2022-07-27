#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}

void EnemyBullet::Update()
{
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer <= 0)
	{
		isDead_ = true;
	}

	// 行列更新	// 発射は更新の前に行う,削除→発射→更新の順となる
	{
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// スケーリング
		worldTransform_.matWorld_ *= MathUtility::Matrix4Scaling(
			worldTransform_.scale_.x,
			worldTransform_.scale_.y,
			worldTransform_.scale_.z
		);

		// 回転
		worldTransform_.matWorld_ *=
			MathUtility::Matrix4RotationZ(worldTransform_.rotation_.z);
		worldTransform_.matWorld_ *=
			MathUtility::Matrix4RotationX(worldTransform_.rotation_.x);
		worldTransform_.matWorld_ *=
			MathUtility::Matrix4RotationY(worldTransform_.rotation_.y);

		// 平行移動
		worldTransform_.matWorld_ *= MathUtility::Matrix4Translation(
			worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z
		);

		// 行列の転送
		worldTransform_.TransferMatrix();
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = TRUE;
}
