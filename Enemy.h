#pragma once
#include "Model.h"

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	// ���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;

	// ���f���̃|�C���^
	Model* model_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

};
