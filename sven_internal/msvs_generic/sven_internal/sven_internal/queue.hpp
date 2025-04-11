/**
 * Copyright - xWhitey, 2024.
 * queue.hpp - an implementation of blocking queue
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef QUEUE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in queue.hpp
#else //QUEUE_HPP_RECURSE_GUARD

#define QUEUE_HPP_RECURSE_GUARD

#ifndef QUEUE_HPP_GUARD
#define QUEUE_HPP_GUARD
#pragma once

#ifdef __cplusplus

#include <mutex>
#include <condition_variable>
#include <deque>

template<class _Ty> struct CQueue {
	std::mutex m_Mtx;
	std::condition_variable m_cvCondition;
	std::deque<_Ty> m_Deque;

	void Push(_Ty const& _Rhs) {
		{
			std::unique_lock<std::mutex> lock(m_Mtx);
			m_Deque.push_front(_Rhs);
		}
		m_cvCondition.notify_one();
	}
	_Ty Pop() {
		std::unique_lock<std::mutex> lock(m_Mtx);
		m_cvCondition.wait(lock, [=] { return !m_Deque.empty(); });
		_Ty result(std::move(m_Deque.back()));
		m_Deque.pop_back();

		return result;
	}
	bool IsExhausted() {
		std::unique_lock<std::mutex> lock(m_Mtx);

		return m_Deque.empty();
	}
};

#else //!__cplusplus
#error C++ compiler required to compile queue.hpp
#endif //__cplusplus

#endif //QUEUE_HPP_GUARD

#undef QUEUE_HPP_RECURSE_GUARD
#endif //QUEUE_HPP_RECURSE_GUARD