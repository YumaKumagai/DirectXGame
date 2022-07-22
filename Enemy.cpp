#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("./Resources/enemy01.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	// �������W�ݒ�
	worldTransform_.translation_ = { 0,2,50 };
}

void Enemy::Update()
{
	// �ړ�����
	worldTransform_.translation_ += Vector3(0, 0, -0.25f);

	// �s��X�V
	{
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// �X�P�[�����O
		{
			Matrix4 matScale = MathUtility::Matrix4Scaling(
				worldTransform_.scale_.x,
				worldTransform_.scale_.y,
				worldTransform_.scale_.z
			);
			worldTransform_.matWorld_ *= matScale;
		}

		// ��]
		{
			Matrix4 matRot = MathUtility::Matrix4RotationZ(worldTransform_.rotation_.z);
			matRot *= MathUtility::Matrix4RotationX(worldTransform_.rotation_.x);
			matRot *= MathUtility::Matrix4RotationY(worldTransform_.rotation_.y);
			worldTransform_.matWorld_ *= matRot;
		}

		// ���s�ړ�
		{
			Matrix4 matTrans = MathUtility::Matrix4Translation(
				worldTransform_.translation_.x,
				worldTransform_.translation_.y,
				worldTransform_.translation_.z
			);
			worldTransform_.matWorld_ *= matTrans;
		}

		// �s��̓]��
		worldTransform_.TransferMatrix();
	}

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
