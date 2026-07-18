#pragma once
//#include "../../Library/gameObject.h"
#include "../objData.h"
#include "../ObjectBase/CharaBase.h"

static const float JUMP_SPEED = 20.0f;
static const float JUMP_GRAVITY = 0.75f;
static const float BOOST_SPEED = 25.0f;
static const float ANGULAR_SPEED = 0.1f;//攻撃の角度変える速度
static const float ATK_RANGE = 125;//範囲
static const float PLAYER_DAMAGE = 10;//10
static const int PLAYER_HP_MAX = 50;
static const float PLAYER_SPEED = 3;
static const int HIT_STOOP = 15;//20

static const float VZERO = 10.0f;
static const float THETA = 30.0f;
static const float GRAVITY = 9.8f;

namespace
{
	static const float PHP_BAR_SIZE_X = 512;//ゲージ
	static const float PHP_BAR_SIZE_Y = 256 / 4;//ゲージ
	static const int PHP_BAR_SIZE = 834 / 2;
	static const float PHP_SIZE = 175 / 2;
	static const float PHP_SPEED = 3.0f;//赤ゲージの削れる速度
}

//class Character;
class Floor;
class IsHit;
class Enemy;
class EffectManager;

class Player : public CharaBase/*public GameObject*/
{
public:
	Player(SceneBase* _scene);
	~Player();
	void Update() override;
	void Draw() override;
	VECTOR GetPosition() const { return position; }
	float GetDirection() const { return direction; }
	bool IsDead() { return isDead; }

	void StartPlay();

	void StopPlay();

	void Damage(int _damage);
	void LaserDamage();

	void HitLaser();

private:

	enum class STATE 
	{
		ST_STOP,
		ST_PLAY,
		ST_JUMP,
		ST_ATK,
		ST_DAMAGE,
		ST_LASER
	};
	STATE state;

	/*VECTOR position;*/
	VECTOR velocity;
	float direction;
	//Character* chara;
	float rotx, rotz;

	void UpdatePlay();	
	void JumpMove();
	void Atk();
	void DamageMove();//ダメージを食らった時の処理
	void RetGauge();//赤ゲージの削れる処理
	void DeadEff();

	Floor* floor;

	XINPUT_STATE key;

	bool isBoost;
	VECTOR _boost;
	int boostCounter;

	VECTOR velocityCopy;

	bool keyCheck;//長押し防止

	float turnDirection;//攻撃時の回転
	int turnIndex;

	float myHp;//hpバー
	Data2D hpBar;
	float hpState;//
	bool isDead;//死んだかそうでないか

	bool isDamage;//ダメージをくらってるかどう
	int hitCounter;
	bool isAtkDamage;//プレイヤーが攻撃を当てた時

	IsHit* isHit;
	Enemy* enemy;
	EffectManager* effect;
	VECTOR copyPos;

	float retHp;//赤HPのやつ

	Vector2 hitPos; //hpの揺れ

	int atkSound;//攻撃した時の音
	int hitSound;//当たった時の音
	int jumpSound;//ジャンプした時の音
	int boostSound;//ブースとした時の音

	int turnCounter;

	float targetDirection;

	float LaserDamagePos;

};