/**
 * Copyright - xWhitey, 2023.
 * functional_lite_type_traits.hpp - A weird implementation of <type_traits> stl's header.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_TYPE_TRAITS_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_type_traits.hpp
#else //FUNCTIONAL_LITE_TYPE_TRAITS_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_TYPE_TRAITS_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_TYPE_TRAITS_HPP_GUARD
#define FUNCTIONAL_LITE_TYPE_TRAITS_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include "functional_lite_config.hpp"

#include FUNCTIONAL_FAKE_SAL2

#include "functional_lite_stdbool.hpp"

FUNCTIONAL_NAMESPACE_BEGIN
	template<class _Ty, _Ty _Value> struct integral_constant {
		static constexpr _Ty value = _Value;
		typedef _Ty value_type;
		typedef integral_constant type;
		constexpr operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; }
	};

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

	template<class _Ty, class _Ty2> struct is_same : false_type {};
	template<class _Ty> struct is_same<_Ty, _Ty> : true_type {};
	template<class _Ty, class _Ty2> constexpr bool is_same_v = is_same<_Ty, _Ty2>::value;

	template<bool _Condition, class _Ty = void> struct enable_if {};
	template<class _Ty> struct enable_if<true, _Ty> { typedef _Ty type; };
	template<bool _Condition, class _Ty = void> using enable_if_t = typename enable_if<_Condition, _Ty>::type;

	template<class T> struct is_pointer : false_type {};
	template<class T> struct is_pointer<T*> : true_type {};
	template<class T> struct is_pointer<T* const> : true_type {};
	template<class T> struct is_pointer<T* volatile> : true_type {};
	template<class T> struct is_pointer<T* const volatile> : true_type {};
	template<class T> constexpr bool is_pointer_v = is_pointer<T>::value;

	template<class _Ty> struct remove_reference { typedef _Ty type; };
	template<class _Ty> struct remove_reference<_Ty&> { typedef _Ty type; };
	template<class _Ty> struct remove_reference<_Ty&&> { typedef _Ty type; };

	template<class _Ty> using remove_reference_t = typename remove_reference<_Ty>::type;

	template<class _Ty> struct remove_pointer { typedef _Ty type; };
	template<class _Ty> struct remove_pointer<_Ty*> { typedef _Ty type; };
	template<class _Ty> struct remove_pointer<_Ty* const> { typedef _Ty type; };
	template<class _Ty> struct remove_pointer<_Ty* volatile> { typedef _Ty type; };
	template<class _Ty> struct remove_pointer<_Ty* const volatile> { typedef _Ty type; };
	template<class _Ty> using remove_pointer_t = typename remove_pointer<_Ty>::type;

	template<bool _Condition, class _Ty = void, class _Ty2 = void, class = enable_if_t<!is_same_v<_Ty, _Ty2>>> struct conditional {};

	template<class _Ty, class _Ty2> struct conditional<true, _Ty, _Ty2> { typedef _Ty type; };
	template<class _Ty, class _Ty2> struct conditional<false, _Ty, _Ty2> { typedef _Ty2 type; };

	template<bool _Condition, class _Ty, class _Ty2, class = enable_if_t<!is_same_v<_Ty, _Ty2>>> using conditional_t = typename conditional<_Condition, _Ty, _Ty2>::type;

	//Simple constexpr bool. Though we don't use it ourselves. - xWhitey
	template<bool _Condition> using constexpr_bool = integral_constant<bool, _Condition>;
	/* Usage:
	constexpr constexpr_bool<true == false> someConstexprBool;
	*/

	template<class _Ty> struct is_array : false_type {};
	template<class _Ty> struct is_array<_Ty[]> : true_type {};
	template<class _Ty, int _Size> struct is_array<_Ty[_Size]> : true_type {};

	template<class _Ty> struct remove_extent { typedef _Ty type; };
	template<class _Ty> struct remove_extent<_Ty[]> { typedef _Ty type; };
	template<class _Ty, int _Size> struct remove_extent<_Ty[_Size]> { typedef _Ty type; };

	template<class _Ty> struct CForceNoDeduction { typedef _Ty type; };

	template<class _Ty> struct add_pointer {
		typedef typename CForceNoDeduction<typename remove_reference<_Ty>::type*>::type type;
	};

	template<class _Ty> struct remove_const { typedef _Ty type; };
	template<class _Ty> struct remove_const<const _Ty> { typedef _Ty type; };

	template<class _Ty> struct remove_volatile { typedef _Ty type; };
	template<class _Ty> struct remove_volatile<volatile _Ty> { typedef _Ty type; };

	template<class _Ty> struct remove_cv {
		typedef _Ty type;

		template <template <class> class _Fn> using _Apply = _Fn<_Ty>;
	};
	template<class _Ty> struct remove_cv<const _Ty> {
		typedef _Ty type;

		template <template <class> class _Fn> using _Apply = _Fn<_Ty>;
	};
	template<class _Ty> struct remove_cv<volatile _Ty> {
		typedef _Ty type;

		template <template <class> class _Fn> using _Apply = _Fn<_Ty>;
	};
	template<class _Ty> struct remove_cv<const volatile _Ty> {
		typedef _Ty type;

		template <template <class> class _Fn> using _Apply = _Fn<_Ty>;
	};

	template<class _Ty> struct is_const : false_type {};
	template<class _Ty> struct is_const<const _Ty> : true_type {};

	template<class _Ty> struct is_reference : false_type {};
	template<class _Ty> struct is_reference<_Ty&> : true_type {};
	template<class _Ty> struct is_reference<_Ty&&> : true_type {};

	template<class _Ty> struct is_rvalue_reference : false_type {};
	template<class _Ty> struct is_rvalue_reference<_Ty&&> : true_type {};
	template<class _Ty> constexpr bool is_rvalue_reference_v = is_rvalue_reference<_Ty>::value;

	//not recommended for use
	template<class _Ty> struct is_function : integral_constant<bool, !is_const<const _Ty>::value && !is_reference<_Ty>::value> {};

	template<class _Ty> struct decay {
	private:
		typedef typename remove_reference<_Ty>::type U;
	public:
		typedef functional::conditional_t<functional::is_array<U>::value, typename functional::add_pointer<typename functional::remove_extent<U>::type>::type, functional::conditional_t<is_function<U>::value, typename functional::add_pointer<U>::type, typename functional::remove_cv<U>::type>> type;
	};

	template<class _Ty> using decay_t = typename decay<_Ty>::type;

	template <class> constexpr bool is_lvalue_reference_v = false;

	template <class _Ty> constexpr bool is_lvalue_reference_v<_Ty&> = true;

	template <class _Ty> constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) {
		return static_cast<_Ty&&>(_Arg);
}

	template <class _Ty> constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) {
		Q_static_assert(!is_lvalue_reference_v<_Ty> && "functional_lite's type_traits: bad \"forward\" call");

		return static_cast<_Ty&&>(_Arg);
	}

	template <class _Ty> constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) {
		return static_cast<remove_reference_t<_Ty>&&>(_Arg);
	}

	//TODO: implement make_signed, make_unsigned
FUNCTIONAL_NAMESPACE_END

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_type_traits.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_TYPE_TRAITS_HPP_GUARD

#undef FUNCTIONAL_LITE_TYPE_TRAITS_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_TYPE_TRAITS_HPP_RECURSE_GUARD