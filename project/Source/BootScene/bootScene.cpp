#include "bootScene.h"
#include "../../Library/sceneManager.h"
#include "../Common/debugScreen.h"
#include "../Common/Effect/effectManager.h"
//#include "fader.h"

BootScene::BootScene()
{
	SceneBase* common = SceneManager::CommonScene();

	//DebugScreen* ds = common->CreateGameObject<DebugScreen>();
	//common->SetDrawOrder(ds, 10000);

	EffectManager* effect = common->CreateGameObject<EffectManager>();
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TitleScene"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
}

void BootScene::Draw()
{
}
