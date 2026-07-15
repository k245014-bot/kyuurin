#pragma once
#include "../Library/gameObject.h"

static const int EFFECT_MAX = 100;

class EffectBase;
class EffectManager :public GameObject
{
public:
	EffectManager(SceneBase* _scene);
	~EffectManager();
	void Update()override;
	void Draw()override;

	enum  EffectID
	{
		PLAYERATK_EFFECT,
		ENEMYATK_EFFECT,
		LESER_EFFECT,
		PLAYER_DEAD,
		ENEMY_PHASE2,
		ENEMY_PHASE3,
		ENEMY_BOON,
		EFFECT_NUM,
	};

	void Creat(VECTOR _pos, EffectID _id, float _speed, VECTOR _size, int _time,int index);
	//0 = プレイヤーのダメージ , 1 = エネミーのダメージ , 2 = レーザー, 3 = プレイヤーの爆散, 4 = 第二段階, 5 = 第三段階, 6 えねみーぼーん
	void SetPos(VECTOR _pos, int index);
	void SetRot(VECTOR _rot, int index);

	bool IsLaser() { return isLaser; }

	void EffDestroy(int index);

private:

	int effectDate[EffectID::EFFECT_NUM];

	EffectBase* effect[EFFECT_MAX];

	bool isLaser;
	int copyTime;
};
