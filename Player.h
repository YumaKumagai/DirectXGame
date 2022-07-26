#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "PlayerBullet.h"
#include <memory>

#define TO_RADIAN(deg) (deg*M_PI/180)

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
#pragma region Update�����\�b�h

	/// <summary>
	/// Y����]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �\���L�[�ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ���e���ˁE�X�V����
	/// </summary>
	void Attack();

#pragma endregion

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection_);

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
	// �e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// �Փ˔��a
	float radius_ = 1.f;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// �V���O���g���C���X�^���X�p�|�C���^
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	// �e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

};
