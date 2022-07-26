#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// ���[���h���W���擾
	Vector3 GetWorldPosition()const
	{
		return Vector3(
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2]
		);
	}
	// �Փ˔��a���擾
	float GetRadius()const { return radius_; }
	// �f�X�t���O���擾
	bool IsDead()const { return isDead_; }

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// �Փ˔��a
	float radius_ = 1.f;
	// ���x�x�N�g��
	Vector3 velocity_;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;
	// ���f���̃|�C���^
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// ����<frm>
	static const int32_t kLifeTime = 60 * 5;

};