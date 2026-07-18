#include "CharaBase.h"

CharaBase::CharaBase(SceneBase* _scene) : MovableObject(_scene)
{
	offsets[MODEL_BODY]  = CharaPos.bodyPos;
	offsets[MODEL_HEAD]  = CharaPos.headPos;
	offsets[MODEL_RHAND] = CharaPos.rHandPos;
	offsets[MODEL_LHAND] = CharaPos.lHandPos;
	offsets[MODEL_RFOOT] = CharaPos.rFootPos;
	offsets[MODEL_LFOOT] = CharaPos.lFootPos;

	runFlag = false;
	footRotation = 0;
}

CharaBase::~CharaBase()
{
	for (int m = 0; m < MODEL_NUM; m++)
	{
		if (hModels[m] > 0) 
			MV1DeleteModel(hModels[m]);
	}
}

void CharaBase::Update()
{
	if (runFlag)
	{
		footRotation += FootSpeed;
		if (footRotation >= DX_PI_F * 2.0f)
			footRotation -= DX_PI_F * 2.0f;
	}
	else
	{
		if (footRotation > 0)
		{
			footRotation += FootSpeed;
			if (footRotation >= DX_PI_F * 2.0f)
				footRotation = 0;
		}
	}

	float r = sinf(footRotation) * DX_PI_F / 2.0f;
	MATRIX rotM = MGetRotX(r);
	VECTOR adds = VTransform(VGet(0, -20.0f, 0), rotM);
	offsets[MODEL_RFOOT] = VAdd(adds, VGet(-9.0f, 28.0f, 0));

	rotM = MGetRotX(-r);
	adds = VTransform(VGet(0, -20.0f, 0), rotM);
	offsets[MODEL_LFOOT] = VAdd(adds, VGet(9.0f, 28.0f, 0));
}

void CharaBase::Draw()
{
	for (int m = 0; m < MODEL_NUM; m++)
	{
		if (hModels[m] > 0)
		{
			VECTOR add = VTransform(offsets[m], GetRotationMatrix());
			MATRIX trs = MGetTranslate(VAdd(position, add));
			MV1SetMatrix(hModels[m], MMult(GetRotationMatrix(), trs));
			MV1DrawModel(hModels[m]);
		}
	}
}

void CharaBase::SetModel(std::string CharaName)
{
	std::string name = "data/model/pg_" + CharaName;

	hModels[MODEL_ID::MODEL_BODY]	= MV1LoadModel((name + "_body.mv1").c_str());
	hModels[MODEL_ID::MODEL_HEAD]	= MV1LoadModel((name +"_head.mv1").c_str());
	hModels[MODEL_ID::MODEL_RHAND]	= MV1LoadModel((name +"_right_hand.mv1").c_str());
	hModels[MODEL_ID::MODEL_LHAND]	= MV1LoadModel((name +"_left_hand.mv1").c_str());
	hModels[MODEL_ID::MODEL_RFOOT]	= MV1LoadModel((name +"_right_foot.mv1").c_str());
	hModels[MODEL_ID::MODEL_LFOOT]	= MV1LoadModel((name +"_left_foot.mv1").c_str());
}

void CharaBase::RunAnim(float speed)
{
	//footSpeed = speed * DX_PI_F / 180.0f;
	runFlag = true;
}

void CharaBase::RunStopAnim()
{
	runFlag = false;
}

void CharaBase::Reset()
{
	offsets[MODEL_RFOOT] = CharaPos.rFootPos;
	offsets[MODEL_LFOOT] = CharaPos.lFootPos;
	footRotation = 0;
}

void CharaBase::HitCharacter(MODEL_ID index, int materialNum, bool shine)
{
	for (int i = 0; i < materialNum; i++)
	{
		if(shine)
			MV1SetMaterialAmbColor(hModels[index], i, GetColorF(1.0f, 0, 0, 1.0f));
		else
			MV1SetMaterialAmbColor(hModels[index], i, GetColorF(0, 0, 0, 0));
	}
}

void CharaBase::AllHitCharacter(bool shine)
{
	HitCharacter(MODEL_ID::MODEL_HEAD, 2, shine);
	HitCharacter(MODEL_ID::MODEL_BODY, 2, shine);
	HitCharacter(MODEL_ID::MODEL_RHAND, 1, shine);
	HitCharacter(MODEL_ID::MODEL_LHAND, 1, shine);
	HitCharacter(MODEL_ID::MODEL_RFOOT, 1, shine);
	HitCharacter(MODEL_ID::MODEL_LFOOT, 1, shine);
}
