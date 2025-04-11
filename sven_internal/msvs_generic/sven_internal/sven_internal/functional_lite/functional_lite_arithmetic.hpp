/**
 * Copyright - xWhitey, 2023.
 * functional_lite_arithmetic.hpp - Our implementation of arithmetic operators (INTEGER ONLY)
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_ARITHMETIC_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_arithmetic.hpp
#else //FUNCTIONAL_LITE_ARITHMETIC_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_ARITHMETIC_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_ARITHMETIC_HPP_GUARD
#define FUNCTIONAL_LITE_ARITHMETIC_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_stddef.hpp"
#include "functional_lite_stdbool.hpp"
#include "functional_lite_stdnull.hpp"

#include "functional_lite_math.hpp"

int Q_abs(_In_ int _Number) {
	return _Number < 0 ? -_Number : _Number;
}

inline int Q_add(_In_ int _From, _In_ int _What) {
	int partialSum, carry;

	do {
		partialSum = _From ^ _What;
		carry = (_From & _What) << 1;
		_From = partialSum;
		_What = carry;
	} while (carry != 0);

	return partialSum;
}

inline int Q_sub(_In_ int _From, _In_ int _What) {
	return Q_add(_From, Q_add(~_What, 1));
}

inline int Q_mul(_In_ int _What, _In_ int _HowMuch) {
	int result = 0;

	if (_HowMuch > 0) {
		for (int idx = 0; idx < _HowMuch; idx++) {
			result = Q_add(result, _What);
		}
	}
	else {
		for (int idx = _HowMuch; idx < 0; idx++) {
			result = Q_sub(result, _What);
		}
	}

	return result;
}

inline int Q_shl(_In_ int _What, _In_ int _HowMuch) {
	int multiplier = Q_pow(2, _HowMuch);

	return Q_mul(_What, multiplier);
}

inline int Q_div(_In_ int _ToDivide, _In_ int _Divisor, _In_opt_ int* _Remainder = nullptr) {
	int quotient = 1;

	bool negative = false;
	if ((_ToDivide > 0 && _Divisor < 0) || (_ToDivide < 0 && _Divisor > 0))
		negative = true;

	int tempdividend = Q_abs(_ToDivide);
	int tempdivisor = Q_abs(_Divisor);

	if (tempdivisor == tempdividend) {
		if (_Remainder) {
			*_Remainder = 0;
		}

		return negative ? -1 : 1;
	}
	else if (tempdividend < tempdivisor) {
		if (_Remainder) {
			if (_ToDivide < 0) {
				*_Remainder = Q_mul(tempdividend, negative ? -1 : 1);
			}
			else {
				*_Remainder = tempdividend;
			}
		}
		return 0;
	}

	while (Q_shl(tempdivisor, 1) <= tempdividend) {
		tempdivisor = Q_shl(tempdivisor, 1);
		quotient = Q_shl(quotient, 1);
	}

	if (_ToDivide < 0) {
		quotient = Q_mul(quotient, (negative ? -1 : 1));
		quotient = Q_add(quotient, Q_div(Q_mul(Q_sub(tempdividend, tempdivisor), -1), _Divisor, _Remainder));
	}
	else {
		quotient = Q_mul(quotient, (negative ? -1 : 1));
		quotient = Q_add(quotient, Q_div(Q_sub(tempdividend, tempdivisor), _Divisor, _Remainder));
	}

	return quotient;
}

inline int Q_shr(_In_ int _What, _In_ int _HowMuch) {
	int divisor = Q_pow(2, _HowMuch);

	return Q_div(_What, divisor);
}

inline int Q_negotiate(_In_ int _What) {
	return Q_mul(Q_add(_What, 1), -1);
}

inline int Q_xor(_In_ int _What, _In_ int _HowMuch) {
	return (_What | _HowMuch) & Q_negotiate(_What & _HowMuch);
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_arithmetic.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_ARITHMETIC_HPP_GUARD

#undef FUNCTIONAL_LITE_ARITHMETIC_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_ARITHMETIC_HPP_RECURSE_GUARD