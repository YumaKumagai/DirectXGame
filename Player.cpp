#include "Player.h"
#include <assert.h>
#include "WinApp.h"
#include "MathUtility.h"

Player::~Player()
{
	delete model_;
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model != nullptr);

	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update()
{

	// ���[���h�s��ύX����
	{
		// �s��ύX�ҋ@
		worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

		// �s��ύX
		{
			// �X�P�[�����O

			// ��]
			Rotate();

			// ���s�ړ�
			Move();

		}

		// �s��ύX�I��
		worldTransform_.TransferMatrix();
	}

	// �e���ˁE�X�V����
	Attack();

	// �f�o�b�O�e�L�X�g
	{
#pragma region define��`

		// ������W
		Vector2 pos = { 50,50 };
		// �f�o�b�O�\���֐�
#define DEBUG_TEXT (debugText_->SetPos(pos.x,pos.y)->Printf)
		// ���s�֐�
#define ENDL() (pos.y+=15)
		// �C���f���g
#define INDENT "		"

#pragma endregion

		// �I�u�W�F�N�g��
		DEBUG_TEXT("Player:");
		ENDL();

		// ���W���
		DEBUG_TEXT(INDENT"Coord=(%f,%f,%f)",
			worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z);
		ENDL();

		// ���˒e��
		DEBUG_TEXT(INDENT"Bullets=%d", bullets_.size());
		ENDL();

#pragma region define���

#undef DEBUG_TEXT
#undef ENDL()
#undef INDENT

#pragma endregion
	}

}

#pragma region Update�����\�b�h

void Player::Rotate()
{
	// ��]�p���x(�ʓx�@)
	const float rotSpeed = TO_RADIAN(3.f);
	// ��]�L�[
	const BYTE
		rightRotKey = DIK_E,
		leftRotKey = DIK_Q;

	// �L�[�{�[�h���͂ɂ���]����
	{
		// �E��]
		worldTransform_.rotation_.y += rotSpeed * input_->PushKey(rightRotKey);
		// ����]
		worldTransform_.rotation_.y -= rotSpeed * input_->PushKey(leftRotKey);
	}

	// ��]�s��̏�Z
	{
		// �s�񏉊���
		Matrix4 matRotY =
			MathUtility::Matrix4RotationY(worldTransform_.rotation_.y);

		// �s��̏�Z
		worldTransform_.matWorld_ *= matRotY;
	}

}

void Player::Move()
{
	// �ړ����x
	const float moveSpeed = 0.5f;
	// �\���L�[
	const BYTE
		upKey = DIK_UP,
		downKey = DIK_DOWN,
		rightKey = DIK_RIGHT,
		leftKey = DIK_LEFT;

	// �L�[�{�[�h���͂ɂ��ړ�����
	{
		int axisX = 0, axisY = 0;

		// �\���L�[���玲�l���擾
		axisY += bool(input_->PushKey(upKey));
		axisY -= bool(input_->PushKey(downKey));
		axisX += bool(input_->PushKey(rightKey));
		axisX -= bool(input_->PushKey(leftKey));

		// �ړ��x�N�g�����Z
		worldTransform_.translation_.x +=
			moveSpeed * float(axisX);
		worldTransform_.translation_.y +=
			moveSpeed * float(axisY);

	}

	// �ړ��͈͂𐧌�
	{
		// �ړ����E���W
		const Vector2 kMoveLimit(34, 18);

		// �͈͂𒴂��Ȃ�����
		worldTransform_.translation_.x =
			max(min(worldTransform_.translation_.x, +kMoveLimit.x), -kMoveLimit.x);
		worldTransform_.translation_.y =
			max(min(worldTransform_.translation_.y, +kMoveLimit.y), -kMoveLimit.y);

	}

	// ���s�ړ��s��̏�Z
	{
		// �s�񏉊���
		Matrix4 matTrans = MathUtility::Matrix4Translation(
			worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z);

		// �s��̏�Z
		worldTransform_.matWorld_ *= matTrans;

	}

}

void Player::Attack()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	// ���e���ˏ���
	if (input_->TriggerKey(DIK_SPACE))
	{
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = MathUtility::Vector3Transform(velocity, worldTransform_.matWorld_);

		// �e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// �e��o�^����
		bullets_.push_back(std::move(newBullet));
	}

	// ���e�X�V����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

}

#pragma endregion

void Player::Draw(const ViewProjection& viewProjection_)
{
	// ���L�����`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// �e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}