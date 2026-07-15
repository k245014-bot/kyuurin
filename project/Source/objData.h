#pragma once

struct Vector2
{
	float x, y;
};

struct Data2D
{
	Vector2 pos;
	int image;
};  

inline Vector2 V2Get(const float x,const float y)
{
	Vector2 _pos;
	_pos.x = x;
	_pos.y = y;

	return _pos;
}