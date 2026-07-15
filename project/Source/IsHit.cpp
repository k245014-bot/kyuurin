#include "isHit.h"

bool IsHit::_IsHit(VECTOR _pos1, VECTOR _pos2,float _scale)
{
	if (VSize(VSub(_pos1, _pos2)) < _scale)
	{
		return true;
	}
	else
	{
		return false;
	}
}
