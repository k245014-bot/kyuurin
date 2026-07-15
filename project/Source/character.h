#pragma once
#include "../Library/gameObject.h"

class Character {
public:
	Character();
	~Character();
	void Update();
	void Draw();

	enum MODEL_ID {
		MODEL_BODY = 0,
		MODEL_HEAD,
		MODEL_RHAND,
		MODEL_LHAND,
		MODEL_RFOOT,
		MODEL_LFOOT,
		MODEL_NUM,
	};
	void SetModel(MODEL_ID id, int handle);
	void SetPosition(VECTOR pos);
	void SetRotation(VECTOR rot);

	void Run(float speed);
	void Stop();

	void PlayerAtk();
	//void 発行する(モデルのデータ, 頭と体が２、それ以外が１bool 光るか光らないか)
	void HitCharacter(MODEL_ID index, int materialNum, bool shine);
	void Reset();
private:
	int hModels[MODEL_NUM];
	VECTOR offsets[MODEL_NUM];

	VECTOR position;
	MATRIX rotation;
	VECTOR scale;
	
	VECTOR atkRot;
	bool copied;//攻撃したときに一回だけコピーするため

	bool runFlag;
	float footRotation;
	float footSpeed;
};