#pragma once
#include "../../Library/gameObject.h"

class Floor : public GameObject 
{
public:
	Floor(SceneBase* _scene);
	~Floor();
	void Draw() override;

	bool IsFloor(VECTOR pos);

	VECTOR SetPlayerPos(VECTOR _pos);

	VECTOR GetFloor();

private:
	int hImage; // °‚Ì‰æ‘œ
	int hSky;   // ‹ó‚Ìƒ‚ƒfƒ‹
};