#include "inputManager.h"

InputManager::InputManager(SceneBase* scene) : GameObject(scene)
{
	SetMousePoint(MOUSE_OFFSET_X, MOUSE_OFFSET_Y);
	isCheckkey = false;
	keyCounter = 0;

	if (GetJoypadNum() != 0)
		isController = true;
	else
		isController = false;

	mouseMoveX = 0;
	mouseMoveY = 0;

	restraint = RESTAINT;

	min = VECTOR2(0, 0);
	max = VECTOR2(MOUSE_OFFSET_X * 2 - 1, MOUSE_OFFSET_Y * 2);
	//SetMouseDispFlag(FALSE);
}

InputManager::~InputManager()
{

}

void InputManager::Update()
{
	if (isController)
		ControllerMove();	//コントローラーのとき
	else  
		KeyBoardMove();		//キーボードのとき
}

void InputManager::ControllerMove()
{
	GetJoypadXInputState(DX_INPUT_PAD1, &controller);

	VECTOR3 InVecL = Normalize(VECTOR3(controller.ThumbLX, controller.ThumbLY, 0));
	VECTOR3 InVecR = Normalize(VECTOR3(controller.ThumbRX, controller.ThumbRY, 0));

	isKey.sickLX = InVecL.x;
	isKey.sickLY = InVecL.y;
	isKey.sickRX = InVecR.x;
	isKey.sickRY = InVecR.y;

	if (controller.Buttons[XINPUT_BUTTON_B] && !isCheckkey)//攻撃ボタン
		isKey.isAtk = CheckKey();
	else
		isKey.isAtk = KeyCoolTime();
}

void InputManager::KeyBoardMove()//updata
{
	//上下移動
	if (CheckHitKey(KEY_INPUT_W))
		isKey.sickLY = 1;
	else if (CheckHitKey(KEY_INPUT_S))
		isKey.sickLY = -1;
	else
		isKey.sickLY = 0;

	//左右移動
	if (CheckHitKey(KEY_INPUT_D))
		isKey.sickLX = 1;
	else if (CheckHitKey(KEY_INPUT_A))
		isKey.sickLX = -1;
	else
		isKey.sickLX = 0;

	//カメラ移動
	// マウスポインタの位置を取得する
	GetMousePoint(&mouseX, &mouseY);

	//画面の中心から動いていた分だけマウスが移動したことになる。
	mouseMoveX = mouseX - mouseCopy.x;
	mouseMoveY = mouseY - mouseCopy.y;

	// マウスポインタの位置を画面中央に設定する
	if (mouseX <= min.x || mouseX >= max.x || mouseY <= min.y || mouseY >= max.y)
	{
		SetMousePoint(MOUSE_OFFSET_X, MOUSE_OFFSET_Y);
		mouseCopy = VECTOR2(MOUSE_OFFSET_X, MOUSE_OFFSET_Y);
	}
	else
		mouseCopy = VECTOR2(mouseX, mouseY);

	//マウスの移動量を格納する。そのまま入れるとハイセンシ(感度が良）すぎるので割った数値を入れる
	isKey.sickRX = mouseMoveX / restraint;
	isKey.sickRY = mouseMoveY / restraint;

	//攻撃ボタン
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !isCheckkey)
		isKey.isAtk = CheckKey();
	else
		isKey.isAtk = KeyCoolTime();

	if(CheckHitKey(KEY_INPUT_SPACE))
		isKey.isJump = CheckKey();
	else
		isKey.isJump = KeyCoolTime();
}

bool InputManager::CheckKey()
{
	isCheckkey = true;
	return true;
}

VECTOR3 InputManager::Normalize(VECTOR3 vec)
{
	if (vec.Size() >= 6000)
		if (vec.Size() >= 32767) //最大まで倒している
			return vec.Normalize();
		else
			return vec / vec.Size();
	else// 入力がないとみなす
		return VZero;
}

bool InputManager::KeyCoolTime()
{
	if (!isCheckkey)
		return false;

	keyCounter++;
	if (keyCounter >= KEYCOOLTIME)
	{
		isCheckkey = false;
		keyCounter = 0;
	}
	return false;
}