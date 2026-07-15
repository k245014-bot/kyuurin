#include "camera.h"
#include "playScene.h"
#include "player.h"

Camera::Camera(SceneBase * _scene) : GameObject(_scene)
{
	position = VGet(0, 100.0f, -500.0f);
	target = VGet(0,0,0);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	Playing();
}

void Camera::Draw()
{
	SetCameraPositionAndTarget_UpVecY(position, target);
}

void Camera::Playing()
{
	Player* player = GetScene()->FindGameObject<Player>();
	VECTOR pPos = player->GetPosition();
	float pDir = player->GetDirection();
	MATRIX m = MGetRotY(pDir);
	target = VTransform(VGet(0, 100.0f, 200.0f), m);
	target = VAdd(target, pPos);
	VECTOR pos = VTransform(VGet(0, 150.0f, -300.0f), m);
	pos = VAdd(pos, pPos); // カメラ位置を計算する

	VECTOR diff = VSub(pos, position);
	position = VAdd(position, VScale(diff, 0.1f)); // カメラ位置を滑らかに補間
}