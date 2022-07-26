#pragma once
#include "Model.h"
#include "EnemyBullet.h"
#include <memory>
#include "Player.h"

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
#pragma region Update�������\�b�h

	// �ڋ߃t�F�[�Y����������
	void InitializeApproach();

	// �ڋ߃t�F�[�Y�X�V����
	void UpdateApproach();

	// ���E�t�F�[�Y�X�V����
	void UpdateLeave();

	// �e����
	void Fire();

#pragma endregion

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// player�o�^
	/// </summary>
	/// <param name="player"></param>
	static void SetPlayer(const Player* player);

private:
	// ���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;

	// ���f���̃|�C���^
	Model* model_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// �s���t�F�[�Y
	enum class Phase
	{
		Init,		// �����t�F�[�Y
		Approach,	// �ڋ�
		Leave,		// ���E
	} phase_ = Phase::Init;

	// �e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// ���ˊԊu
	static const int kFireInterval = 60;

	// ���˃^�C�}�[
	int32_t fireTimer_ = 0;

	// player�Q�Ɨp�|�C���^
	static const Player* kPlayer_;

};

