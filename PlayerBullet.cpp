#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	// NULLポインタチェック
	assert(model != nullptr);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/black.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

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
				Matrix4 matTrans = MathUtility::Matrix4Translation(
					worldTransform_.translation_.x,
					worldTransform_.translation_.y,
					worldTransform_.translation_.z);

				// 行列の乗算
				worldTransform_.matWorld_ *= matTrans;

			}

		}

		// 行列変更終了
		worldTransform_.TransferMatrix();
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
