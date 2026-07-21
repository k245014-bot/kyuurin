#pragma once
#include "../objData.h"
#include "../ObjectBase/CharaBase.h"

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

	const bool GetIsDead() { return isDead; }

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

	const float JumpSpeed = 20.0f;
	const float JumpGravity = 0.75f;
	const float BoostSpeed = 25.0f;
	//攻撃の角度変える速度
	const float AngularSpeed = 0.1f;
	//範囲
	const float AtkRange = 125;
	const float PlayerDamage = 10;
	const int playerHpMax = 50;
	const float PlayerSpeed = 10.0f;
	const int HitStoop = 15;
	const float Theta = 30.0f;
	const float Gravity = 9.8f;

	const VECTOR3 SpawnPos = VECTOR3(-100.0f, 0, -200.0f);
	//キャラクターを滑るように見せるための数値
	const float Deceleration = 0.9f;
	const float PlayerMoveSpeed = 1.0f;
	const float AngleOffset = DX_PI_F * 0.5f;
	const float InputOffset = DX_PI_F * 1.5f;
	const float AngleAtkMax = AngularSpeed * 10;
	const float AtkPlayerSpeed = PlayerSpeed * 0.7f;
	const float DamageMoveSpeed = PlayerSpeed / 5;
	//ゲージ
	const float HpBarSizeX = 512;
	const float HpBarSizeY = 256 / 4;

	const int HpBarSize = 834 / 2;
	const float HpSize = 175 / 2;
	//赤ゲージの削れる速度
	const float RetHpSpeed = 3.0f;

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