#pragma once

#include <DxLib.h>

inline float ToRadian(float deg) { return deg * DX_PI_F / 180.0f; }
inline float ToDegree(float rad) { return rad / DX_PI_F * 180.0f; }

inline VECTOR& operator +=(VECTOR& v1, const VECTOR& v2) { return v1 = VAdd(v1, v2); }
inline VECTOR operator +(const VECTOR& v1, const VECTOR& v2) { return VAdd(v1, v2); }
inline VECTOR& operator -=(VECTOR& v1, const VECTOR& v2) { return v1 = VSub(v1, v2); }
inline VECTOR operator -(const VECTOR& v1, const VECTOR& v2) { return VSub(v1, v2); }
inline VECTOR& operator *=(VECTOR& v1, float scale) { return v1 = VScale(v1, scale); }
inline VECTOR operator *(VECTOR& v1, float scale) { return VScale(v1, scale); }
inline VECTOR& operator /=(VECTOR& v1, float scale) { return v1 = VScale(v1, 1.0f/scale); }
inline VECTOR operator /(VECTOR& v1, float scale) { return VScale(v1, 1.0f/scale); }
inline VECTOR& operator *=(VECTOR& v1, const MATRIX& m1) { return v1 = VTransform(v1, m1); }
inline VECTOR operator *(VECTOR& v1, const MATRIX& m1) { return VTransform(v1, m1); }

inline MATRIX& operator *=(MATRIX& m1, const MATRIX& m2) { return m1 = MMult(m1, m2); }
inline MATRIX operator *(const MATRIX& m1, const MATRIX& m2) { return MMult(m1, m2); }
