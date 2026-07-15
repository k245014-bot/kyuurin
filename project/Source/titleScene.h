#pragma once
#include "../Library/sceneBase.h"

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>

class TitleScene : public SceneBase {
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;
private:
	int hLogo; // タイトルロゴ画像

	bool pushed; // Pキーを押したか
	int counter; // 画面演出用カウンター

	int rin;
	int kyu;
	int kyuPos;
	int rinPos;

	int hBG;

	int cCounter;
};
