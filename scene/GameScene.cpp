﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;

	// 自キャラの解放
	delete player_;

	// 敵の解放
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(isDebugCameraActive_);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// モデルの生成
	model_ = Model::Create();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, TextureManager::Load("mario.jpg"));

	// 自キャラ登録
	Enemy::SetPlayer(player_);
	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	enemy_->Initialize(model_);

}

void GameScene::Update() {

#ifdef _DEBUG
#pragma region デバッグカメラ・軸表示の切り替えとそれに伴うビュープロジェクションの更新
	// デバッグカメラ・軸方向表示の有効フラグをトグル
	if (input_->TriggerKey(DIK_LSHIFT))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
		AxisIndicator::GetInstance()->SetVisible(isDebugCameraActive_);
	}
	//カメラの処理
	if (isDebugCameraActive_)
	{
		// デバッグカメラの更新
		debugCamera_->Update();
		// ビュープロジェクションにコピー
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	}
	else
	{
		// ビュープロジェクション行列の再計算と転送
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}
#pragma endregion
#endif

	// 自キャラの更新
	player_->Update();

	// 敵の更新
	if (enemy_ != nullptr)
	{
		enemy_->Update();
	}

	// 衝突判定と応答
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	if (enemy_ != nullptr)
	{
		enemy_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 判定対象AとBの半径
	float radiusA, radiusB;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	// 自キャラと敵弾の当たり判定
	{
		// 自キャラの座標と半径
		posA = player_->GetWorldPosition();
		radiusA = player_->GetRadius();

		// 自キャラと敵弾全ての当たり判定
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
		{
			// 敵弾の座標と半径
			posB = bullet->GetWorldPosition();
			radiusB = bullet->GetRadius();

			// 球同士が当たっていれば
			if (CollisionSphereAAndSphereB(posA, radiusA, posB, radiusB))
			{
				// 衝突時コールバックを呼び出す
				player_->OnCollision();
				bullet->OnCollision();

			}

		}

	}

	// 自弾と敵キャラの当たり判定
	{
		// 敵キャラの座標と半径
		posA = enemy_->GetWorldPosition();
		radiusA = enemy_->GetRadius();

		// 自弾と敵キャラの当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			// 自弾の座標と半径
			posB = bullet->GetWorldPosition();
			radiusB = bullet->GetRadius();

			// 球同士が当たっていれば
			if (CollisionSphereAAndSphereB(posA, radiusA, posB, radiusB))
			{
				// 衝突時コールバックを呼び出す
				enemy_->OnCollision();
				bullet->OnCollision();

			}
		}

	}

	// 自弾と敵弾の当たり判定
	{
		for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets)
		{
			// 自弾の座標と半径
			posA = playerBullet->GetWorldPosition();
			radiusA = playerBullet->GetRadius();

			for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets)
			{
				// 敵弾の座標と半径
				posB = enemyBullet->GetWorldPosition();
				radiusB = enemyBullet->GetRadius();

				// 球同士が当たっていれば
				if (CollisionSphereAAndSphereB(posA, radiusA, posB, radiusB))
				{
					// 衝突時コールバックを呼び出す
					playerBullet->OnCollision();
					enemyBullet->OnCollision();

				}
			}

		}

	}

}
#pragma region 衝突判定メソッド

// 球Aと球Bの当たり判定
bool GameScene::CollisionSphereAAndSphereB(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB)const
{
	return
		powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2)
		<= powf(radiusA + radiusB, 2);
}

#pragma endregion