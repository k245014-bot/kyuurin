#pragma once
#include "../../Library/gameObject.h"

class MovableObject :public GameObject
{
public:
	MovableObject(SceneBase* _scene);
	virtual ~MovableObject();

	void Update()override;
	void Draw()override;

	const VECTOR3 GetPosition() { return position; }
	const VECTOR3 GetRotation() { return rotation; }
protected:
	MATRIX GetRotationMatrix() const;
	//Ç‹Ç¡Ç∑ÇÆêiÇﬁ
	void NormalMove(float _speed);
	//éwíËÇµÇΩï˚å¸Ç…Ç∑Ç∑Çﬁ
	void Move(VECTOR3 _vector, float _speed);

	VECTOR3 CalcMoveVector(float _angle, float _speed);
	////éwíËÇµÇΩï˚å¸Ç…Ç∑Ç∑Çﬁ
	//void Move(VECTOR3 _vector,float _rotY, float _speed);

	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
};