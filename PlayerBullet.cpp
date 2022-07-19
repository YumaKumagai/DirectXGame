#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	// NULL�|�C���^�`�F�b�N
	assert(model != nullptr);

	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("./Resources/black.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

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
				Matrix4 matTrans = MathUtility::Matrix4Translation(
					worldTransform_.translation_.x,
					worldTransform_.translation_.y,
					worldTransform_.translation_.z);

				// �s��̏�Z
				worldTransform_.matWorld_ *= matTrans;

			}

		}

		// �s��ύX�I��
		worldTransform_.TransferMatrix();
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
