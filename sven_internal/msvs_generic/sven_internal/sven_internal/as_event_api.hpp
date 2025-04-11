/**
 * Copyright - xWhitey, 2024.
 * as_event_api.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef AS_EVENT_API_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in as_event_api.hpp
#else //AS_EVENT_API_HPP_RECURSE_GUARD

#define AS_EVENT_API_HPP_RECURSE_GUARD

#ifndef AS_EVENT_API_HPP_GUARD
#define AS_EVENT_API_HPP_GUARD
#pragma once

#ifdef __cplusplus

template<class _Ty, class _To> _To* EventCast(_Ty* _From) {
	if (!_From) return 0;

	return reinterpret_cast<_To*>(_From);
}

#else //!__cplusplus
#error C++ compiler required to compile as_event_api.hpp
#endif //__cplusplus

#endif //AS_EVENT_API_HPP_GUARD

#undef AS_EVENT_API_HPP_RECURSE_GUARD
#endif //AS_EVENT_API_HPP_RECURSE_GUARD