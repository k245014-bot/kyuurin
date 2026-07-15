#include "effect.h"
#include <EffekseerForDXLib.h>

EffectBase::EffectBase(SceneBase* _scene):GameObject(_scene)
{
	position = VGet(0, 0, 0);
	speed = 0;
	size = VGet(0, 0, 0);
	time = 0;

	active = false;
}

EffectBase::~EffectBase()
{

}

void EffectBase::Update()
{
	if (!active)
		return;
	//SetScalePlayingEffekseer3DEffect(playingEffect, size.x, size.y, size.z);
	SetSpeedPlayingEffekseer3DEffect(playingEffect, speed);

	time--;
	if (time <= 0)
	{
		active = false;
		StopEffekseer3DEffect(playingEffect);
		DestroyMe();
	}
}

void EffectBase::Draw()
{
}

void EffectBase::Creat(VECTOR _pos, int _effectId, float _speed, VECTOR _size, int _time)
{
	if (active)
		return;
	else
	{
		position = _pos;
		hEffect = _effectId;
		speed = _speed;
		size = _size;
		time = _time;

		active = true;

		playingEffect = PlayEffekseer3DEffect(hEffect);
		SetPosPlayingEffekseer3DEffect(playingEffect, position.x, position.y, position.z);
	}
}

void EffectBase::SetPos(VECTOR _pos)
{
	SetPosPlayingEffekseer3DEffect(playingEffect,_pos.x, _pos.y, _pos.z);
}

void EffectBase::SetRotation(VECTOR _rot)
{
	SetRotationPlayingEffekseer3DEffect(playingEffect, _rot.x, _rot.y, _rot.z);
}

void EffectBase::Active()
{
	active = false;
}

void EffectBase::Destroy()
{
	StopEffekseer3DEffect(playingEffect);
}

