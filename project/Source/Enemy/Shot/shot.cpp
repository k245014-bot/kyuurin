#include "shot.h"
#include "../../IsHit.h"
#include "../../Player/player.h"

Shot::Shot(SceneBase* _scene) :GameObject(_scene)
{
	deleteCounter = 0;
}

void Shot::Update()
{
	if (!active)
		return;
	deleteCounter++;

	velocity.z += cosf(direction) * 0.2f * 2;
	velocity.x += sinf(direction) * 0.2f * 2;

	position = VAdd(position, velocity);
	if (deleteCounter >= DELETE_COUNTER)
	{
		active = false;
		deleteCounter = 0;
	}
}

void Shot::Draw()
{
	if(active)
		DrawSphere3D(position, 30.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);//15
}

bool Shot::CreateShot(VECTOR _pos, VECTOR _vel, float _dir)
{
	if (active)
		return false;

	active = true;

	position = _pos;//
	position.y = POSITION_Y;
	velocity = _vel;
	direction = _dir;

	return true;
}
