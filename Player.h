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
#pragma region Update�������\�b�h

	/// <summary>
	/// Y����]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �\���L�[�ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ���W���f�o�b�O�e�L�X�g�ŕ\��
	/// </summary>
	void DisplayCoord(float posX, float posY)const;

	/// <summary>
	/// ���e���ˁE�X�V����
	/// </summary>
	void Attack();

#pragma endregion

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection_);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
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
