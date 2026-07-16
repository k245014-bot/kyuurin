#include "floor.h"
#include <assert.h>

namespace
{
	static const float XS = 1500;//3000
	static const float YS = -50;
	static const float ZS = 1500;
	static const COLOR_U8 DIF = GetColorU8(128,128,128,255);
	static const COLOR_U8 SPC = GetColorU8(8,8,8,255);
};

Floor::Floor(SceneBase* _scene) : GameObject(_scene)
{
	hImage = LoadGraph("data\\texture\\tatami.png");
	assert(hImage>0);

	hSky = MV1LoadModel("data\\model\\sky\\Stage00_sky.mv1");
	assert(hSky>0);
}

Floor::~Floor()
{
	DeleteGraph(hImage);
	MV1DrawModel(hSky);
}

void Floor::Draw()
{
	SetBackgroundColor(128,128,128);
	static const VERTEX3D vertexes[] = {
		{ {-XS,  0, -ZS}, VGet(0,1.0f,0), DIF, SPC, 0, 0, 0, 0 },
		{ {-XS,  0,  ZS}, VGet(0,1.0f,0), DIF, SPC, 0, 1.0f, 0, 0 },
		{ { XS,  0, -ZS}, VGet(0,1.0f,0), DIF, SPC, 1.0f, 0, 0, 0 },
		{ {-XS,  0,  ZS}, VGet(0,1.0f,0), DIF, SPC, 0, 1.0f, 0, 0 },
		{ { XS,  0,  ZS}, VGet(0,1.0f,0), DIF, SPC, 1.0f, 1.0f, 0, 0 },
		{ { XS,  0, -ZS}, VGet(0,1.0f,0), DIF, SPC, 1.0f, 0, 0, 0 },

		{ { XS, YS,  ZS}, VGet(0,-1.0f,0), DIF, SPC, 0, 0, 0, 0 },
		{ { XS, YS, -ZS}, VGet(0,-1.0f,0), DIF, SPC, 0, 1.0f, 0, 0 },
		{ {-XS, YS,  ZS}, VGet(0,-1.0f,0), DIF, SPC, 1.0f, 0, 0, 0 },
		{ { XS, YS, -ZS}, VGet(0,-1.0f,0), DIF, SPC, 0, 1.0f, 0, 0 },
		{ {-XS, YS, -ZS}, VGet(0,-1.0f,0), DIF, SPC, 1.0f, 1.0f, 0, 0 },
		{ {-XS, YS,  ZS}, VGet(0,-1.0f,0), DIF, SPC, 1.0f, 0, 0, 0 },

		{ {-XS,  0, -ZS}, VGet(0,0,-1.0f), DIF, SPC, 0, 0, 0, 0 },
		{ { XS,  0, -ZS}, VGet(0,0,-1.0f), DIF, SPC, 0.01f, 0, 0, 0 },
		{ {-XS, YS, -ZS}, VGet(0,0,-1.0f), DIF, SPC, 0, 0.01f, 0, 0 },
		{ {-XS, YS, -ZS}, VGet(0,0,-1.0f), DIF, SPC, 0, 0.01f, 0, 0 },
		{ { XS,  0, -ZS}, VGet(0,0,-1.0f), DIF, SPC, 0.01f, 0, 0, 0 },
		{ { XS, YS, -ZS}, VGet(0,0,-1.0f), DIF, SPC, 0.01f, 0.01f, 0, 0 },

		{ { XS,  0,  ZS}, VGet(0,0,1.0f), DIF, SPC, 0, 0, 0, 0 },
		{ {-XS,  0,  ZS}, VGet(0,0,1.0f), DIF, SPC, 0.01f, 0, 0, 0 },
		{ { XS, YS,  ZS}, VGet(0,0,1.0f), DIF, SPC, 0, 0.01f, 0, 0 },
		{ { XS, YS,  ZS}, VGet(0,0,1.0f), DIF, SPC, 0, 0.01f, 0, 0 },
		{ {-XS,  0,  ZS}, VGet(0,0,1.0f), DIF, SPC, 0.01f, 0, 0, 0 },
		{ {-XS, YS,  ZS}, VGet(0,0,1.0f), DIF, SPC, 0.01f, 0.01f, 0, 0 },

		{ {-XS,  0, -ZS}, VGet(-1.0f,0,0), DIF, SPC, 0, 0, 0, 0 },
		{ {-XS,  0,  ZS}, VGet(-1.0f,0,0), DIF, SPC, 0.01f, 0, 0, 0 },
		{ {-XS, YS, -ZS}, VGet(-1.0f,0,0), DIF, SPC, 0, 0.01f, 0, 0 },
		{ {-XS, YS, -ZS}, VGet(-1.0f,0,0), DIF, SPC, 0, 0.01f, 0, 0 },
		{ {-XS,  0,  ZS}, VGet(-1.0f,0,0), DIF, SPC, 0.01f, 0, 0, 0},
		{ {-XS, YS,  ZS}, VGet(-1.0f,0,0), DIF, SPC,0.01f, 0.01f, 0, 0},

		{ { XS,  0,  ZS}, VGet(1.0f,0,0), DIF, SPC, 0, 0, 0, 0 },
		{ { XS,  0, -ZS}, VGet(1.0f,0,0), DIF, SPC, 0.01f, 0, 0, 0 },
		{ { XS, YS,  ZS}, VGet(1.0f,0,0), DIF, SPC, 0, 0.01f, 0, 0 },
		{ { XS, YS,  ZS}, VGet(1.0f,0,0), DIF, SPC, 0, 0.01f, 0, 0 },
		{ { XS,  0, -ZS}, VGet(1.0f,0,0), DIF, SPC, 0.01f, 0, 0, 0 },
		{ { XS, YS, -ZS}, VGet(1.0f,0,0), DIF, SPC, 0.01f, 0.01f, 0, 0 },
	};
	DrawPolygon3D(vertexes, 12, hImage, FALSE);
	MV1DrawModel(hSky);
}

bool Floor::IsFloor(VECTOR pos)
{
	return false;
}

VECTOR Floor::SetPlayerPos(VECTOR _pos)
{
	if (_pos.x < -XS )
		_pos.x = -XS ;
	if (_pos.x > XS )
		_pos.x = XS ;
	
	if (_pos.z < -ZS)
		_pos.z = -ZS;
	if (_pos.z > ZS)
		_pos.z = ZS;
	
	return _pos;
}

VECTOR Floor::GetFloor()
{
	return VGet(XS, YS, ZS);
}

