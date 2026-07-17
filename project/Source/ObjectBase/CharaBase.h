#pragma once
#include "MovableObject.h"

class CharaBase :public MovableObject
{
public:
	CharaBase();
	virtual ~CharaBase();

	void Update()override;
	void Draw()override;
protected:
	enum MODEL_ID
	{
		MODEL_BODY = 0,
		MODEL_HEAD,
		MODEL_RHAND,
		MODEL_LHAND,
		MODEL_RFOOT,
		MODEL_LFOOT,
		MODEL_NUM,
	};
	struct CharaPos
	{
		VECTOR3 bodyPos;
		VECTOR3 headPos;
		VECTOR3 rHandPos;
		VECTOR3 lHandPos;
		VECTOR3 rFootPos;
		VECTOR3 lFootPos;

		CharaPos()
		{
			bodyPos  = VECTOR3(0, 24.0f, 0);
			headPos  = VECTOR3(0, 46.0f, 0);
			rHandPos = VECTOR3(-22.0f, 27.0f, 0);
			lHandPos = VECTOR3(22.0f, 27.0f, 0);
			rFootPos = VECTOR3(-7.0f, 8.0f, 0);
			lFootPos = VECTOR3(7.0f, 8.0f, 0);
		}
	};
	const CharaPos CharaPos;
	/// <summary>
	/// どのモデルをロードするか決める
	/// </summary>
	/// <param name="CharaName">blueかredを入れる</param>
	void SetModel(std::string CharaName);
	//走ってるアニメーションを再生
	void RunAnim(float speed);
	//走ってるアニメーションを止める
	void RunStopAnim();
	//足をデフォルトの位置に戻す
	void Reset();

	int hModels[MODEL_NUM];
	VECTOR3 offsets[MODEL_NUM];

	bool runFlag;
	float footRotation;
	const float FootSpeed = 10.0f * DX_PI_F / 180.0f;
};

