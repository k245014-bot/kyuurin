#pragma once
#include "../ObjectBase/MovableObject.h"

static const float Boss = 2.0f;

class InputManager;

class Camera : public MovableObject
{
public:
	Camera(SceneBase* _scene);
	~Camera();
	void Update() override;

	void SetPlayerPosition(const VECTOR& pos);

	void DebugCam();
	//セッター
	void DeadPlayer(bool isDead);
private:
	InputManager* input;
	VECTOR lookPosition;
	void DeadMove();

	bool deadPlayer;

	float deadPosY;
	float pi;
};
//class Camera : public GameObject
//{
//public:
//	Camera(SceneBase* _scene);
//	~Camera();
//	void Update() override;
//	void Draw() override;
//	
//private:
//	/// <summary>
//	/// プレイ中のカメラ移動
//	/// </summary>
//	void Playing();
//
//	VECTOR position;
//	VECTOR target;
//};