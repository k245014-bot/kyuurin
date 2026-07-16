#pragma once
#include "../../../Library/gameObject.h"

static const int DELETE_COUNTER = 180;
static const float POSITION_Y = 30;
static const int SHOT_DAMAGE = 10;

class IsHit;
class Player;

class Shot :public GameObject
{
public:
	Shot(SceneBase* _scene);
	//~Shot();
	void Update();
	void Draw();

	bool CreateShot(VECTOR _pos,VECTOR _vel,float _dir);

	bool IsActive() { return active; }
	VECTOR GetPosition() { return position; }
private:
	VECTOR position;
	VECTOR velocity;
	bool active;

	float direction;//Œü‚«
	int deleteCounter;

	IsHit* isHit;
	Player* player;
};