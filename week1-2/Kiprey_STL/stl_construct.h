#pragma once
#include <new.h>
#include "stl_type_traits.h"
#include "stl_iterator.h"

__STL_BEGIN_NAMESPACE

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __value) {
	new ((void*)__p) _T1(__value);
}

template <class _T1>
inline void _Construct(_T1* __p) {
	new ((void*)__p) _T1();
}

template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
	__pointer->~_Tp();
}


template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type) {}

template <class _ForwardIterator, class _Tp>
inline void
__destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
	typedef typename __type_traits<_Tp>::has_trivial_destructor
		_Trivial_destructor;
	__destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {
	__destroy(__first, __last, __VALUE_TYPE(__first));
}

inline void _Destroy(char*, char*) {}
inline void _Destroy(int*, int*) {}
inline void _Destroy(long*, long*) {}
inline void _Destroy(float*, float*) {}
inline void _Destroy(double*, double*) {}


template <class _T1, class _T2>
inline void construct(_T1* __p, const _T2& __value) {
	_Construct(__p, __value);
}

template <class _T1>
inline void construct(_T1* __p) {
	_Construct(__p);
}

template <class _Tp>
inline void destroy(_Tp* __pointer) {
	_Destroy(__pointer);
}

template <class _ForwardIterator>
inline void destroy(_ForwardIterator __first, _ForwardIterator __last) {
	_Destroy(__first, __last);
}

template <class _ForwardIterator>
void
__destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type)
{
	for (; __first != __last; ++__first)
		destroy(&*__first);
}


__STL_END_NAMESPACE

