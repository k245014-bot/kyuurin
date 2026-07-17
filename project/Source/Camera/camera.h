#pragma once
#include "../../Library/gameObject.h"

class Camera : public GameObject
{
public:
	Camera(SceneBase* _scene);
	~Camera();
	void Update() override;
	void Draw() override;
	
private:
	/// <summary>
	/// ƒvƒŒƒC’†‚ÌƒJƒƒ‰ˆÚ“®
	/// </summary>
	void Playing();

	VECTOR position;
	VECTOR target;
};