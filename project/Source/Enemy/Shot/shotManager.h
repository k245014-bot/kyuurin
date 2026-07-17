#pragma once
#include "../../../Library/gameObject.h"

static const int SHOT_MAX = 1000;
//static const int SHOT_DAMAGE = 10;

class Shot;
class IsHit;
class Player;

class ShotManager :public GameObject
{
public:
	ShotManager(SceneBase* _scene);
	~ShotManager();
	void Update()override;
	void Draw()override;

	void CreateShot(VECTOR _pos, VECTOR _vel, float _dir);

private:
	Shot* shot[SHOT_MAX];
	IsHit* isHit;
	Player* player;
};