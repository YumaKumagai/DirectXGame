#include "Enemy.h"
#include <cassert>

const Player* Enemy::kPlayer_;

void Enemy::Initialize(Model* model)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemy01.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	// �������W�ݒ�
	worldTransform_.translation_ = { 0,2,50 };

	// �ڋ߃t�F�[�Y�ɐݒ�
	phase_ = Phase::Approach;

	// �ڋ߃t�F�[�Y������
	InitializeApproach();

}

void Enemy::Update()
{
	// �e�폜,�X�V
	{
		// �f�X�t���O�̗������e���폜
		bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
			{
				return bullet->IsDead();
			});

		// �e�̍X�V����
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
		{
			bullet->Update();
		}

	}

	// �t�F�[�Y�ʍX�V����
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

void Enemy::InitializeApproach()
{
	// ���˃^�C�}�[��������
	fireTimer_ = kFireInterval;

}

void Enemy::UpdateApproach()
{
	// �ړ����x
	const Vector3 approachVelocity(0, 0, -0.125f);

	// �ړ�
	worldTransform_.translation_ += approachVelocity;
	// �K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}

	// �e����
	Fire();

}

void Enemy::UpdateLeave()
{
	// �ړ����x
	const Vector3 leaveVelocity(-0.1f, 0.1f, -0.1f);

	// �ړ�
	worldTransform_.translation_ += leaveVelocity;
}

void Enemy::Fire()
{
	assert(kPlayer_ != nullptr);

	// �e�̑��x
	const float kBulletSpeed = 1.0f;

	// ���˃^�C�}�[�J�E���g�_�E��
	if (fireTimer_ > 0)
	{
		fireTimer_--;
	}
	// �w�莞�ԂɒB����
	else
	{
		// �e����
		{
			// �e�̑��x�x�N�g��
			Vector3 velocity;
			// �e�̑��x�x�N�g�������߂�
			{
				// ���[���h���W�擾
				Vector3 worldPos(
					worldTransform_.matWorld_.m[3][0],
					worldTransform_.matWorld_.m[3][1],
					worldTransform_.matWorld_.m[3][2]
				);
				// �G�����̍����x�N�g��
				Vector3 DiffVec = kPlayer_->GetWorldPosition() - worldPos;
				// �����x�N�g���̐��K��
				MathUtility::Vector3Normalize(DiffVec);

				// �e�̑��x�x�N�g��
				velocity = kBulletSpeed * DiffVec;
			}

			// �e�𐶐����A������
			std::unique_ptr<EnemyBullet> newBullet =
				std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_,
				velocity);

			// �e��o�^����
			bullets_.push_back(std::move(newBullet));
		}

		// ���˃^�C�}�[��������
		fireTimer_ = kFireInterval;

	}

}

#pragma endregion

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�̕`�揈��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::SetPlayer(const Player* player)
{
	assert(player != nullptr);
	kPlayer_ = player;
}
