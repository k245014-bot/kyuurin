#pragma once
//#include "objectBase.h"
#include "../../Library/gameObject.h"

static const int KEYCOOLTIME = 20;
static const int STICK_MAX = 32767;
static const int MOUSE_OFFSET_X = 1920 / 2;		//画面の中央座標。マウスの移動量計測に使用
static const int MOUSE_OFFSET_Y = 1080 / 2;
static const int RESTAINT = 10;//抑制

struct KeyPush
{
public:
	float sickLX;
	float sickLY;
	float sickRX;
	float sickRY;

	bool isAtk;				//プレイヤーが攻撃キーを押したとき
	bool isJump;			//プレイヤー1ジャンプキーを推したとき

	KeyPush()
	{
		sickLX = 0;
		sickLY = 0;
		sickRX = 0;
		sickRY = 0;

		isAtk = false;
		isJump = false;
	}
};

class InputManager :public GameObject
{
public:
	InputManager(SceneBase* _scene);
	~InputManager();
	void Update()/*override*/;

	//ゲッター
	KeyPush GetKeyPush() { return isKey; }
	//セッター

	friend class MoveKeyboard;

private:
	XINPUT_STATE controller;
	int mouseX, mouseY;

	void KeyBoardMove();			//キーボード操作
	void ControllerMove();			//コントローラー操作
	bool CheckKey();				//ボタンを押したあとの処理
	VECTOR3 Normalize(VECTOR3 vec);
	bool KeyCoolTime();				//ボタンを再度押せるようにする関数

	bool isController;
	bool isCheckkey;				//ボタンが押されているか
	int keyCounter;
	int mouseMoveX;					//カーソルX軸
	int mouseMoveY;					//カーソルY軸
	int restraint;					//抑制

	VECTOR2 min;
	VECTOR2 max;
	VECTOR2 mouseCopy;
	KeyPush isKey;
};
