#include "camera.h"
#include "../playScene.h"
#include "../Player/player.h"
#include "../Common/Imput/inputManager.h"

Camera::Camera(SceneBase* _scene) : MovableObject(_scene)
{
	input = GetScene()->FindGameObject<InputManager>();

	lookPosition = VZero;
	rotation.x = 25.0f * DegToRad;
	rotation.y = 0/*225 * DegToRad*/;

	deadPlayer = false;

	deadPosY = 0;
	pi = 0;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (!deadPlayer)
	{
		rotation.y += 1.5f * input->GetKeyPush().sickRX / 20.0f;
	}
	else
		DeadMove();

	VECTOR3 offset = VECTOR3(0.0f, 400.0f + deadPosY, 300.0f);
	offset = offset * MGetRotX(rotation.x) * MGetRotY(rotation.y + DX_PI_F);

	SetCameraPositionAndTarget_UpVecY(
		lookPosition + offset,
		lookPosition + VECTOR3(0.0f, 200.0f, 0.0f));
}

void Camera::SetPlayerPosition(const VECTOR& pos)
{
	lookPosition = pos;
}

void Camera::DebugCam()
{
	deadPosY = 2500;
	pi = DX_PI_F;
}

void Camera::DeadPlayer(bool isDead)
{
	deadPlayer = isDead;
}

void Camera::DeadMove()
{
	if (deadPosY <= 800)
		deadPosY += 5.0;
}

//Camera::Camera(SceneBase * _scene) : GameObject(_scene)
//{
//	position = VGet(0, 100.0f, -500.0f);
//	target = VGet(0,0,0);
//}
//
//Camera::~Camera()
//{
//}
//
//void Camera::Update()
//{
//	Playing();
//}
//
//void Camera::Draw()
//{
//	SetCameraPositionAndTarget_UpVecY(position, target);
//}
//
//void Camera::Playing()
//{
//	Player* player = GetScene()->FindGameObject<Player>();
//	VECTOR pPos = player->GetPosition();
//	float pDir = player->GetDirection();
//	MATRIX m = MGetRotY(pDir);
//	target = VTransform(VGet(0, 100.0f, 200.0f), m);
//	target = VAdd(target, pPos);
//	VECTOR pos = VTransform(VGet(0, 150.0f, -300.0f), m);
//	pos = VAdd(pos, pPos); // カメラ位置を計算する
//
//	VECTOR diff = VSub(pos, position);
//	position = VAdd(position, VScale(diff, 0.1f)); // カメラ位置を滑らかに補間
//}