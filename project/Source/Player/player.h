#pragma once
//#include "../../Library/gameObject.h"
#include "../objData.h"
#include "../ObjectBase/CharaBase.h"

 const float JUMP_SPEED = 20.0f;
 const float JUMP_GRAVITY = 0.75f;
 const float BOOST_SPEED = 25.0f;
 const float ANGULAR_SPEED = 0.1f;//攻撃の角度変える速度
 const float ATK_RANGE = 125;//範囲
 const float PLAYER_DAMAGE = 10;
 const int PLAYER_HP_MAX = 50;
 const float PLAYER_SPEED = 10;
 const int HIT_STOOP = 15;

const float THETA = 30.0f;
const float GRAVITY = 9.8f;

namespace
{
	 const float PHP_BAR_SIZE_X = 512;//ゲージ
	 const float PHP_BAR_SIZE_Y = 256 / 4;//ゲージ
	 const int PHP_BAR_SIZE = 834 / 2;
	 const float PHP_SIZE = 175 / 2;
	 const float PHP_SPEED = 3.0f;//赤ゲージの削れる速度
}

//class Character;
class Floor;
class IsHit;
class Enemy;
class EffectManager;
class InputManager;
class Camera;

class Player : public CharaBase
{
public:
	Player(SceneBase* _scene);
	~Player();
	void Update() override;
	void Draw() override;
	VECTOR GetPosition() const { return position; }
	bool IsDead() { return isDead; }

	void StartPlay();

	void StopPlay();

	void Damage(int _damage);
	void LaserDamage();

	void HitLaser();

private:
	IsHit* isHit;
	Enemy* enemy;
	EffectManager* effect;
	InputManager* input;
	Camera* camera;
	Floor* floor;

	const VECTOR3 SpawnPos = VECTOR3(-100.0f, 0, -200.0f);
	//キャラクターを滑るように見せるための数値
	const float Deceleration = 0.9f;
	const float PlayerMoveSpeed = 2.0f;
	const float AngleOffset = DX_PI_F * 0.5f;
	const float InputOffset = DX_PI_F * 1.5f;
	const float AngleAtkMax = ANGULAR_SPEED * 10;
	const float AtkPlayerSpeed = PLAYER_SPEED * 0.7f;
	const float DamageMoveSpeed = PLAYER_SPEED / 5;

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

	VECTOR3 velocity;
	float rotx, rotz;

	void UpdatePlay();	
	void JumpMove();
	void Atk();
	void DamageMove();//ダメージを食らった時の処理
	void RetGauge();//赤ゲージの削れる処理
	void DeadEff();
	void LookAt();
	
	bool isBoost;
	VECTOR _boost;
	int boostCounter;

	VECTOR velocityCopy;

	float turnDirection;//攻撃時の回転
	int turnIndex;

	float myHp;//hpバー
	Data2D hpBar;
	float hpState;//
	bool isDead;//死んだかそうでないか

	bool isDamage;//ダメージをくらってるかどう
	int hitCounter;
	bool isAtkDamage;//プレイヤーが攻撃を当てた時

	VECTOR copyPos;

	float retHp;//赤HP

	Vector2 hitPos; //hpの揺れ

	int atkSound;//攻撃した時の音
	int hitSound;//当たった時の音
	int jumpSound;//ジャンプした時の音
	int boostSound;//ブースとした時の音

	int turnCounter;

	float targetDirection;

	float LaserDamagePos;

	float goalRotY;

	float copyRotY;
};