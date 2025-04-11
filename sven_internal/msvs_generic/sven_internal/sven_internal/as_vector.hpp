/**
 * Copyright - xWhitey, 2024.
 * as_vector.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef AS_VECTOR_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in as_vector.hpp
#else //AS_VECTOR_HPP_RECURSE_GUARD

#define AS_VECTOR_HPP_RECURSE_GUARD

#ifndef AS_VECTOR_HPP_GUARD
#define AS_VECTOR_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CASVector {
	CASVector(float _X, float _Y, float _Z);
	CASVector();

	void AddRef();
	void Release();

	float LengthSqr();
	float Length();
	float Length2DSqr();
	float Length2D();

	CASVector operator*(float _Value);
	CASVector operator/(float _Value);

	CASVector operator+(const CASVector& _Rhs);
	CASVector operator-(const CASVector& _Rhs);

	CASVector& operator=(const CASVector& _Rhs);

	bool operator==(const CASVector& _Rhs);

	float* At(unsigned int _Which);
	float At(unsigned int _Which) const;

	float x, y, z;

	int m_nRefCount;
} CASVector;

using CASVector = struct CASVector;

#else //!__cplusplus
#error C++ compiler required to compile as_vector.hpp
#endif //__cplusplus

#endif //AS_VECTOR_HPP_GUARD

#undef AS_VECTOR_HPP_RECURSE_GUARD
#endif //AS_VECTOR_HPP_RECURSE_GUARD