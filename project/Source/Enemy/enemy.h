#pragma once
#include "../../Library/gameObject.h"
#include "../objData.h"

static const float ENEMY_SPEED = 0.8f;//0.2
static const int XZ = 2;
static const float SHOT_SPEED = 0.1f;
static const float RUN_DEGREE = 30;
static const int COOLTIME = 250;//エネミーの移動までの時間
static const float HP_MAX = 150;//150

namespace
{
	static const float HP_BAR_X = 1024;
	static const float HP_BAR_Y = 128;
	static const int HP_BAR_SIZE = 834;
	static const float HP_SIZE = 175;

	static const float HP_SPEED = 5.0f;
}

class Character;
class Player;
class IsHit;
class Floor;
class ShotManager;
class EffectManager;

class Enemy : public GameObject
{
public:
	Enemy(SceneBase* _scene);
	~Enemy();
	void Update() override;
	void Draw() override;
	VECTOR GetPosition() const { return position; }
	float GetDirection() { return direction; }

	void AddVelocity(VECTOR v);

	void StartPlay();
	void Damage(float _playerDamage);

	void PlayerData(Player* _player);

	bool IsDead() { return isDead; }

	void HitLaser();

	void StStop();

private:
	VECTOR position;
	VECTOR velocity;
	float direction;
	Character* chara;
	float rotx, rotz;

	enum class STATE
	{
		ST_STOP,
		ST_PLAY,
		ST_SHOT,
		ST_DEAD,
		ST_DAMAGE,
	};
	STATE state;

	void EnemyMove();
	void EnemyShot();
	void DeadEnemy();
	void DamageMove();
	void RetGauge();//赤ゲージの削れる処理
	
	Player* player;
	VECTOR playerPos;//プレイヤーのポジションコピー

	bool isAtkMove;///攻撃するまでの移動をするか
	IsHit* isHit;//当たり判定

	VECTOR destination;//移動先
	bool destinationCheck;//移動先の決定をしてもいいか
	int moveCounter[XZ];
	VECTOR target;//行く方向
	float targetDirection;//見る向き

	Floor* floor;

	ShotManager* shot;

	int coolTimeCounter;

	float myHp;//敵のhp
	bool isDead;

	Data2D hpBar;//hpのフレーム
	int hpState;

	int damageCounter;//ダメージくらった後の光る処理のため

	EffectManager* effect;

	float retHp;//赤hp

	int atkSound;

	int coolTime;

	int soundCounter;//一回だけ鳴らすとき
	int laserSound;
	int chargeSound;
	int chageCounter;

	int phaseCounter;
};
