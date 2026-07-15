#pragma once
#include "../Library/gameObject.h"

class Light : public GameObject {
public:
	Light(SceneBase* _scene);
	~Light();
	void Update();
	void Draw();
private:
	int hLight;
};