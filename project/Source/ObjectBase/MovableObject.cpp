#include "MovableObject.h"

MovableObject::MovableObject(SceneBase* _scene) : GameObject(_scene)
{
	position = VZero;
	rotation = VZero;
	scale = VZero;
}

MovableObject::~MovableObject()
{
}

void MovableObject::Update()
{

}

void MovableObject::Draw()
{
}

MATRIX MovableObject::GetRotationMatrix() const
{
	//ã„ó–ÇÃå≥ÉfÅ[É^Ç™îΩì]ÇµÇƒÇÈÇ©ÇÁÇÌÇ¥Ç∆âÒì]Ç≥ÇπÇƒÇÈ
	MATRIX base = MGetRotY(DX_PI_F);
	MATRIX mx = MGetRotX(rotation.x);
	MATRIX my = MGetRotY(rotation.y);
	MATRIX mz = MGetRotZ(rotation.z);

	return MMult(MMult(MMult(base, mx), mz), my);
}

void MovableObject::NormalMove(float _speed)
{
	Move(VECTOR3(0, 0, -1), _speed);
}

void MovableObject::Move(VECTOR3 _vector, float _speed)
{
	position += _vector * MGetRotY(rotation.y) * _speed;
}

VECTOR3 MovableObject::CalcMoveVector(float _angle, float _speed)
{
	return VECTOR3(sinf(_angle) * _speed, 0, cosf(_angle) * _speed);
}

//void MovableObject::Move(VECTOR3 _vector, float _rotY, float _speed)
//{
//	/*_vector.y = 0.0f;*/
//	position += _vector * MGetRotY(_rotY) * _speed;
//}
