#pragma once
#include "../Library/sceneBase.h"

class Player;
class Enemy;
class Camera;
//class Shot;
class ShotManager;

/// <summary>
/// ゲームプレイシーン本体
/// </summary>
class PlayScene : public SceneBase {
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

	/// プレイヤーが落下した
	void PlayerFall();

	/// 敵が落下した
	void EnemyFall();

private:
	enum class STATE 
	{
		PLAY,         // プレイ中
		ENEMY_DEAD,   // 敵が落下中
		WINNER,       // 「かち」表示
		PLAYER_DEAD,  // プレイヤー落下中
		LOSER,        // 「まけ」表示
	};
	STATE state;
	
	int counter;

	Player* player;
	Enemy* enemy;
	Camera* camera;
	//Shot* shot;
	ShotManager* shotManager;

	int hReady;   // 「ようい」「はじめ」の画像
	//int hWinLose; // 「かち」「まけ」の画像	//【追加】--
	int win;
	int lose;

	bool isDead;
	int deadCounter;//どっちかが死んだときのカウンター

	int sound;
};