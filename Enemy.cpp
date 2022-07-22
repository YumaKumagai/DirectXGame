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

	// �ڋ߃t�F�[�Y�ɐݒ�
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
#pragma region Update�������\�b�h

void Enemy::UpdateApproach()
{
	// �ړ����x
	const Vector3 approachVelocity(0, 0, -0.25f);

	// �ړ�
	worldTransform_.translation_ += approachVelocity;
	// �K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave()
{
	// �ړ����x
	const Vector3 leaveVelocity(-0.1f, 0.1f, -0.1f);

	// �ړ�
	worldTransform_.translation_ += leaveVelocity;
}

#pragma endregion

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
