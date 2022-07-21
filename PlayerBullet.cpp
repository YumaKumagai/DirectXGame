#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model != nullptr);
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/black.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	// 引数で受け取ったモデルをセット
	model_ = model;

}

void PlayerBullet::Update()
{
	// ワールドトランスフォームの更新
	{
		// 行列変更待機
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// 行列変更
		{
			// スケーリング
			{
			}

			// 回転
			{
			}

			// 平行移動
			{
				// 座標を移動
				worldTransform_.translation_ += velocity_;

				// 行列の乗算
				Matrix4 matTrans = MathUtility::Matrix4Translation(
					worldTransform_.translation_.x,
					worldTransform_.translation_.y,
					worldTransform_.translation_.z);
				worldTransform_.matWorld_ *= matTrans;

			}

		}

		// 行列変更終了
		worldTransform_.TransferMatrix();
	}

	// 時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
