#pragma once
#include "../objData.h"
#include "../ObjectBase/CharaBase.h"

static const float ENEMY_SPEED = 0.8f;//0.2
static const int XZ = 2;
static const float SHOT_SPEED = 0.1f;
static const float RUN_DEGREE = 30;
static const int COOLTIME = 250;//エネミーの移動までの時間
static const int HP_MAX = 150;//150

namespace
{
	static const int HP_BAR_X = 1024;
	static const int HP_BAR_Y = 128;
	static const int HP_BAR_SIZE = 834;
	static const int HP_SIZE = 175;

	static const float HP_SPEED = 5.0f;
}

class Character;
class Player;
class IsHit;
class Floor;
class ShotManager;
class EffectManager;

class Enemy : public CharaBase
{
public:
	Enemy(SceneBase* _scene);
	~Enemy();
	void Update() override;
	void Draw() override;

	void StartPlay();
	void Damage(float _playerDamage);

	void PlayerData(Player* _player);

	void HitLaser();

	void StStop();

	const bool GetIsDead() { return isDead; }
private:
	Player* player;
	Floor* floor;
	IsHit* isHit;//当たり判定
	EffectManager* effect;

	const VECTOR3 SpawnPos = VECTOR3(100.0f, 0, 200.0f);
	const float AttackRange = 100.0f;
	const float LaserAttack = 0.5f;
	const float LastAttack = 0.1f;
	const float ChargeMaxTime = 50.0f;
	const float LaserMaxTime = 200.0f;

	const int LaserEffTime = 200;
	const int HitEffTime = 100;
	const int PhaseEffTime = 100;
	
	const int LaserCoolTime = 350;
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
	

	VECTOR velocity;
	float rotx, rotz;

	VECTOR playerPos;//プレイヤーのポジションコピー

	bool isAtkMove;///攻撃するまでの移動をするか
	

	VECTOR destination;//移動先
	bool destinationCheck;//移動先の決定をしてもいいか
	int moveCounter[XZ];
	VECTOR target;//行く方向
	float targetDirection;//見る向き

	ShotManager* shot;

	int coolTimeCounter;

	float myHp;//敵のhp
	bool isDead;

	Data2D hpBar;//hpのフレーム
	int hpState;

	int damageCounter;//ダメージくらった後の光る処理のため


	float retHp;//赤hp

	int atkSound;

	int coolTime;

	int soundCounter;//一回だけ鳴らすとき
	int laserSound;
	int chargeSound;
	int chageCounter;

	int phaseCounter;
};
