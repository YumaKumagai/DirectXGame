#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model != nullptr);
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("./Resources/black.png");
	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
	// �����Ŏ󂯎�������f�����Z�b�g
	model_ = model;

}

void PlayerBullet::Update()
{
	// ���[���h�g�����X�t�H�[���̍X�V
	{
		// �s��ύX�ҋ@
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// �s��ύX
		{
			// �X�P�[�����O
			{
			}

			// ��]
			{
			}

			// ���s�ړ�
			{
				// ���W���ړ�
				worldTransform_.translation_ += velocity_;

				// �s��̏�Z
				Matrix4 matTrans = MathUtility::Matrix4Translation(
					worldTransform_.translation_.x,
					worldTransform_.translation_.y,
					worldTransform_.translation_.z);
				worldTransform_.matWorld_ *= matTrans;

			}

		}

		// �s��ύX�I��
		worldTransform_.TransferMatrix();
	}

	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
