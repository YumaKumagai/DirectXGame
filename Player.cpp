#include "Player.h"
#include <assert.h>
#include "WinApp.h"
#include "MathUtility.h"

Player::~Player()
{
	delete model_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLポインタチェック
	assert(model != nullptr);

	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{

	// ワールド行列変更処理
	{
		// 行列変更待機
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// 行列変更
		{
			// 回転
			Rotate();

			// 平行移動
			Move();

		}

		// 行列変更終了
		worldTransform_.TransferMatrix();
	}

	// 弾発射・更新処理
	Attack();

	// デバッグテキスト
	{
		DisplayCoord(50.f, 50.f);
	}
}

#pragma region Update分割メソッド

void Player::Rotate()
{
	// 回転角速度(弧度法)
	const float rotSpeed = TO_RADIAN(6.f);
	// 回転キー
	const BYTE
		rightRotKey = DIK_E,
		leftRotKey = DIK_Q;

	// キーボード入力による回転処理
	{
		// 右回転
		worldTransform_.rotation_.y += rotSpeed * input_->PushKey(rightRotKey);
		// 左回転
		worldTransform_.rotation_.y -= rotSpeed * input_->PushKey(leftRotKey);
	}

	// 回転行列の乗算
	{
		// 行列初期化
		Matrix4 matRotY =
			MathUtility::Matrix4RotationY(worldTransform_.rotation_.y);

		// 行列の乗算
		worldTransform_.matWorld_ *= matRotY;
	}

}

void Player::Move()
{
	// 移動速度
	const float moveSpeed = 0.5f;
	// 十字キー
	const BYTE
		upKey = DIK_UP,
		downKey = DIK_DOWN,
		rightKey = DIK_RIGHT,
		leftKey = DIK_LEFT;

	// キーボード入力による移動処理
	{
		int axisX = 0, axisY = 0;

		// 十字キーから軸値を取得
		axisY += bool(input_->PushKey(upKey));
		axisY -= bool(input_->PushKey(downKey));
		axisX += bool(input_->PushKey(rightKey));
		axisX -= bool(input_->PushKey(leftKey));

		// 移動ベクトル加算
		worldTransform_.translation_.x +=
			moveSpeed * float(axisX);
		worldTransform_.translation_.y +=
			moveSpeed * float(axisY);

	}

	// 移動範囲を制限
	{
		// 移動限界座標
		const Vector2 kMoveLimit(34, 18);

		// 範囲を超えない処理
		worldTransform_.translation_.x =
			max(min(worldTransform_.translation_.x, +kMoveLimit.x), -kMoveLimit.x);
		worldTransform_.translation_.y =
			max(min(worldTransform_.translation_.y, +kMoveLimit.y), -kMoveLimit.y);

	}

	// 平行移動行列の乗算
	{
		// 行列初期化
		Matrix4 matTrans = MathUtility::Matrix4Translation(
			worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z);

		// 行列の乗算
		worldTransform_.matWorld_ *= matTrans;

	}

}

void Player::DisplayCoord(float posX, float posY)const
{
	// 座標情報を描画
	debugText_->SetPos(posX, posY)->
		Printf("Player:(%f,%f,%f)",
			worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z);
}

void Player::Attack()
{
	// 自弾発射処理
	if (input_->TriggerKey(DIK_SPACE))
	{
		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullets_.push_back(std::move(newBullet));
	}

	// 自弾更新処理
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

#pragma endregion

void Player::Draw(const ViewProjection& viewProjection_)
{
	// 自キャラ描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}