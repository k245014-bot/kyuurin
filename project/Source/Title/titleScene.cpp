#include "titleScene.h"
#include "../../Library/sceneManager.h"
#include <assert.h>
#include <DxLib.h>
#include "../config.h"

TitleScene::TitleScene():
	counter(0),pushed(false)
{
	SetBackgroundColor(0,128,0);
	SetCameraPositionAndTarget_UpVecY(VGet(0, 50.0f, -300.0f), VGet(0,150.0f, 0));

	hLogo = LoadGraph("data\\texture\\title2.png");
	assert(hLogo >= 0);

	hBG = LoadGraph("data\\texture\\titleBG.png");

	PlaySoundFile("data\\sound\\title.wav", DX_PLAYTYPE_LOOP);

	rin = LoadGraph("data\\sourceimages\\a_10.png");
	
	kyu = LoadGraph("data\\sourceimages\\b_10.png");

	kyuPos = SCREEN_WIDTH - 300;
	rinPos = 20;
	counter = 0;
	cCounter = 0;
}

TitleScene::~TitleScene()
{
	if(hLogo > 0)
		DeleteGraph(hLogo);
}

void TitleScene::Update()
{
	SceneBase::Update();
	XINPUT_STATE key;
	GetJoypadXInputState(DX_INPUT_PAD1, &key);

	counter++;

	if (!pushed)
	{
		if (CheckHitKey(KEY_INPUT_P) || GetMouseInput() & MOUSE_INPUT_LEFT || key.Buttons[XINPUT_BUTTON_B])
		{
			pushed = true;
			counter = 0;
			StopSoundFile();
		}
	} 
	else
	{
		if (counter >= 100)
		{
			SceneManager::ChangeScene("PlayScene");
		}
	}

	if (cCounter == 0)
	{
		kyuPos -= 10;
		rinPos += 10;
		if (SCREEN_WIDTH - 300 <= rinPos)
			cCounter = 1;
	}
	else
	{
		kyuPos += 10;
		rinPos -= 10;
		if (50 >= rinPos)
			cCounter = 0;
	}
}

void TitleScene::Draw()
{
	SceneBase::Draw();

	DrawGraph(0, 0, hBG, TRUE);

	DrawGraph(kyuPos, 200, rin, TRUE);
	DrawGraph(rinPos, 200, kyu, TRUE);
	DrawGraph(SCREEN_WIDTH / 3 + 50,SCREEN_HEIGHT / 5 + 100, hLogo, TRUE);

	if (!pushed)
	{
		if (counter % 60 < 40)
			DrawString(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT /1.5 , "Push B To Start", GetColor(255, 255, 255));
	}
}
