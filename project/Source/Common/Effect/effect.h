#pragma once
#include "../../Library/gameObject.h"

class EffectBase : public GameObject
{
public:
	EffectBase(SceneBase* _scene);
	~EffectBase();
	void Update()override;
	void Draw()override;

	void Creat(VECTOR _pos, int _effectId, float _speed, VECTOR _size,int _time);
	bool IsActive() { return active; }

	void SetPos(VECTOR _pos);
	void SetRotation(VECTOR _rot);

	void Active();

	void Destroy();

private:
	bool active;//再生するかどうか

	VECTOR position;
	int playingEffect;//変換するやつ
	int hEffect;//エフェクト
	float speed;//再生速度
	VECTOR size;
	int time;//再生する時間
};