#include "light.h"

Light::Light(SceneBase* _scene) : GameObject(_scene)
{
	SetLightEnable(TRUE);
	SetLightDirection(VGet(0.0f, -0.71f, -0.71f));
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));

	hLight = CreateDirLightHandle(VGet(0.0f, -0.71f, 0.71f));
	SetLightDifColorHandle(hLight, GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
}

Light::~Light()
{
	SetLightDirection(VGet(1.0f, -1.0f, 1.0f));
	DeleteLightHandle(hLight);
}


void Light::Update()
{
}

void Light::Draw()
{
}
