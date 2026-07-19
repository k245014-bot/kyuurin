#include "playScene.h"
#include "../Library/sceneManager.h"
#include <assert.h>
#include "Player/player.h"
#include "Enemy/enemy.h"
#include "Camera/camera.h"
#include "Light/light.h"
#include "Floor/floor.h"
#include "Enemy/Shot/shotManager.h"
#include "Common/Imput/inputManager.h"

PlayScene::PlayScene()
{
	CreateGameObject<InputManager>();
	camera = CreateGameObject<Camera>();
	CreateGameObject<Light>();
	CreateGameObject<Floor>();
	enemy = CreateGameObject<Enemy>();
	player = CreateGameObject<Player>();
	shotManager = CreateGameObject<ShotManager>();

	enemy->PlayerData(player);
	hReady = LoadGraph("data\\texture\\readygo.png");
	assert(hReady);

	win = LoadGraph("data\\texture\\Winner.png");
	assert(win);
	lose = LoadGraph("data\\texture\\Haibokusha.png");
	assert(lose);

	state = STATE::PLAY;
	counter = 0;

	isDead = true;
}

PlayScene::~PlayScene()
{
	DeleteGraph(hReady);
	
	DeleteGraph(win);
	DeleteGraph(lose);

}

void PlayScene::Update()
{
	SceneBase::Update();
	switch (state)
	{
	case STATE::PLAY:
		if (++counter == 60)
		{
			player->StartPlay();
			enemy->StartPlay();
			//sound = PlaySoundFile("data\\sound\\play.wav", DX_PLAYTYPE_LOOP);
			sound = LoadSoundMem("data\\sound\\play.wav", DX_PLAYTYPE_LOOP);
			PlaySoundMem(sound, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(200, sound);
		}
		break;
	case STATE::PLAYER_DEAD:
		deadCounter++;
		if (deadCounter >= 120)
			state = STATE::LOSER;
		break;
		break;
	case STATE::ENEMY_DEAD:
		deadCounter++;
		if (deadCounter >= 120)
			state = STATE::WINNER;
		break;

	case STATE::LOSER:
		deadCounter++;
		enemy->StStop();
		if (deadCounter >= 300)
		{
			StopSoundMem(sound);
			SceneManager::ChangeScene("TitleScene");
		}
	case STATE::WINNER:
		deadCounter++;
		if (deadCounter >= 300)
		{
			StopSoundMem(sound);
			SceneManager::ChangeScene("TitleScene");
		}
	}

	if (enemy->IsDead())
	{
		if (isDead)
		{
			state = STATE::ENEMY_DEAD;
			isDead = false;
		}
	}
	if (player->IsDead())
	{
		if (isDead)
		{
			state = STATE::PLAYER_DEAD;
			isDead = false;
		}
	}
}

void PlayScene::Draw()
{
	SceneBase::Draw();
	if (state== STATE::PLAY)
	{
		if (counter<60)
			DrawRectGraph(300, 200, 0, 0, 256, 128, hReady, TRUE, FALSE);
		else if (counter<120)
			DrawRectGraph(300, 200, 0, 128, 256, 128, hReady, TRUE, FALSE);
	} 
	else if (state== STATE::LOSER) 
	{
		DrawRectGraph(50, 250, 0, 0, 1034, 136, lose, TRUE, FALSE);
	} else if (state == STATE::WINNER)
	{
		DrawRectGraph(500, 350, 0, 0, 823, 123, win, TRUE, FALSE);
	}
}

void PlayScene::PlayerFall()
{
	StopSoundFile();
	state = STATE::PLAYER_DEAD;
	counter = 0;
}

void PlayScene::EnemyFall()
{
	StopSoundFile();
	state = STATE::ENEMY_DEAD;
	counter = 0;
	player->StopPlay();
}
