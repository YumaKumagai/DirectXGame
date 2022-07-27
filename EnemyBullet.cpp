#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

}

void EnemyBullet::Update()
{
	// ���W���ړ�������
	worldTransform_.translation_ += velocity_;

	// ���Ԍo�߂Ńf�X
	if (--deathTimer <= 0)
	{
		isDead_ = true;
	}

	// �s��X�V	// ���˂͍X�V�̑O�ɍs��,�폜�����ˁ��X�V�̏��ƂȂ�
	{
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// �X�P�[�����O
		worldTransform_.matWorld_ *= MathUtility::Matrix4Scaling(
			worldTransform_.scale_.x,
			worldTransform_.scale_.y,
			worldTransform_.scale_.z
		);

		// ��]
		worldTransform_.matWorld_ *=
			MathUtility::Matrix4RotationZ(worldTransform_.rotation_.z);
		worldTransform_.matWorld_ *=
			MathUtility::Matrix4RotationX(worldTransform_.rotation_.x);
		worldTransform_.matWorld_ *=
			MathUtility::Matrix4RotationY(worldTransform_.rotation_.y);

		// ���s�ړ�
		worldTransform_.matWorld_ *= MathUtility::Matrix4Translation(
			worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z
		);

		// �s��̓]��
		worldTransform_.TransferMatrix();
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = TRUE;
}
