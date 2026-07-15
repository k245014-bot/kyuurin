#include "character.h"

Character::Character()
{
	for (int m=0; m<MODEL_NUM; m++)
		hModels[m] = -1;
	offsets[MODEL_BODY] = VGet(0, 24.0f, 0);
	offsets[MODEL_HEAD] = VGet(0, 46.0f, 0);
	offsets[MODEL_RHAND] = VGet(-22.0f, 27.0f, 0);
	offsets[MODEL_LHAND] = VGet(22.0f, 27.0f, 0);
	offsets[MODEL_RFOOT] = VGet(-7.0f, 8.0f, 0);
	offsets[MODEL_LFOOT] = VGet( 7.0f, 8.0f, 0);

	position = VGet(0,0,0);
	rotation = MGetIdent();

	atkRot = VGet(0, 0, 0);
	copied = false;

	runFlag = false;
	footRotation = 0;
	footSpeed = 10.0f * DX_PI_F / 180.0f;
}

Character::~Character()
{
	for (int m = 0; m < MODEL_NUM; m++) {
		if (hModels[m]>0) {
			MV1DeleteModel(hModels[m]);
		}
	}
}

void Character::Update()
{
	if (runFlag)
	{
		footRotation += footSpeed;
		if (footRotation >= DX_PI_F * 2.0f)
			footRotation -= DX_PI_F * 2.0f;
	}
	else
	{
		if (footRotation > 0)
		{
			footRotation += footSpeed;
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

void Character::Draw()
{
	for (int m = 0; m < MODEL_NUM; m++)
	{
		if (hModels[m]>0)
		{
			VECTOR add = VTransform(offsets[m], rotation);
			MATRIX trs = MGetTranslate(VAdd(position, add));
			MV1SetMatrix(hModels[m], MMult(rotation, trs));
			MV1DrawModel(hModels[m]);
		}
	}
}

void Character::SetModel(MODEL_ID id, int handle)
{
	hModels[id] = handle;
}

void Character::SetPosition(VECTOR pos)
{
	position = pos;
}

void Character::SetRotation(VECTOR rot)
{
	MATRIX base = MGetRotY(DX_PI_F);
	MATRIX mx = MGetRotX(rot.x);
	MATRIX my = MGetRotY(rot.y);
	MATRIX mz = MGetRotZ(rot.z);
	rotation = MMult(MMult(MMult(base, mx), mz), my);
}

void Character::Run(float speed)
{
	footSpeed = speed * DX_PI_F / 180.0f;
	runFlag = true;
}

void Character::Stop()
{
	runFlag = false;
}

void Character::PlayerAtk()
{
	//offsets[MODEL_RHAND] = VGet(-22.0f, 27.0f, 0);
}

void Character::HitCharacter(MODEL_ID index, int materialNum, bool shine)
{
	for (int i = 0; i < materialNum; i++)
	{
		if(shine)
			MV1SetMaterialAmbColor(hModels[index], i, GetColorF(1.0f, 0, 0, 1.0f));
		else
			MV1SetMaterialAmbColor(hModels[index], i, GetColorF(0, 0, 0, 0));
	}
}

void Character::Reset()
{
	offsets[MODEL_RFOOT] = VGet(-7.0f, 8.0f, 0);
	offsets[MODEL_LFOOT] = VGet(7.0f, 8.0f, 0);
	footRotation = 0;
}
