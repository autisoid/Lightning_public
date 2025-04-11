/**
 * Copyright - xWhitey, 2023.
 * functional_lite_stdnull.hpp - Implementation of NULL (presented as Q_NULL) and nullptr (presented as nullptr)
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_STDNULL_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_stdnull.hpp
#else //FUNCTIONAL_LITE_STDNULL_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_STDNULL_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_STDNULL_HPP_GUARD
#define FUNCTIONAL_LITE_STDNULL_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define Q_NULL reinterpret_cast<void*>(0)

inline namespace {
	constexpr const struct {
		template<class _Ty> operator _Ty*() const {
			return static_cast<_Ty*>(Q_NULL);
		}

		template<class _Ty, class _Holder> operator _Ty _Holder::*() const {
			return static_cast<_Ty*>(Q_NULL);
		}

		operator void*() const {
			return Q_NULL;
		}

	private:
		void operator&() const;
	} Q_nullptr = {};
}

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_stdnull.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_STDNULL_HPP_GUARD

#undef FUNCTIONAL_LITE_STDNULL_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_STDNULL_HPP_RECURSE_GUARD