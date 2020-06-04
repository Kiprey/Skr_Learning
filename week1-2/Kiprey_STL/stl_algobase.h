#pragma once
#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_pair.h"
#include <cstring>

__STL_BEGIN_NAMESPACE


template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*) {
	_Tp __tmp = *__a;
	*__a = *__b;
	*__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b) {
	__iter_swap(__a, __b, __VALUE_TYPE(__a));
}

template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
	_Tp __tmp = __a;
	__a = __b;
	__b = __tmp;
}

template <class _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
	return __b < __a ? __b : __a;
}

template <class _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
	return  __a < __b ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
	return __comp(__b, __a) ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
	return __comp(__a, __b) ? __b : __a;
}


template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
	_OutputIter __result,
	input_iterator_tag, _Distance*)
{
	for (; __first != __last; ++__result, ++__first)
		* __result = *__first;
	return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter
__copy(_RandomAccessIter __first, _RandomAccessIter __last,
	_OutputIter __result, random_access_iterator_tag, _Distance*)
{
	for (_Distance __n = __last - __first; __n > 0; --__n) {
		*__result = *__first;
		++__first;
		++__result;
	}
	return __result;
}


template <class _Tp>
inline _Tp*
__copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result) {
	memmove(__result, __first, sizeof(_Tp) * (__last - __first));
	return __result + (__last - __first);
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
	_OutputIter __result)
{
	return __copy(__first, __last, __result,
		__ITERATOR_CATEGORY(__first),
		__DISTANCE_TYPE(__first));
}

#define __STL_DECLARE_COPY_TRIVIAL(_Tp)                                \
  inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) { \
    memmove(__result, __first, sizeof(_Tp) * (__last - __first));          \
    return __result + (__last - __first);                                  \
  }

__STL_DECLARE_COPY_TRIVIAL(char)
__STL_DECLARE_COPY_TRIVIAL(signed char)
__STL_DECLARE_COPY_TRIVIAL(unsigned char)
__STL_DECLARE_COPY_TRIVIAL(short)
__STL_DECLARE_COPY_TRIVIAL(unsigned short)
__STL_DECLARE_COPY_TRIVIAL(int)
__STL_DECLARE_COPY_TRIVIAL(unsigned int)
__STL_DECLARE_COPY_TRIVIAL(long)
__STL_DECLARE_COPY_TRIVIAL(unsigned long)
__STL_DECLARE_COPY_TRIVIAL(float)
__STL_DECLARE_COPY_TRIVIAL(double)
__STL_DECLARE_COPY_TRIVIAL(long double)

template <class _BidirectionalIter1, class _BidirectionalIter2,
	class _Distance>
	inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first,
		_BidirectionalIter1 __last,
		_BidirectionalIter2 __result,
		bidirectional_iterator_tag,
		_Distance*)
{
	while (__first != __last)
		* --__result = *--__last;
	return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first,
	_RandomAccessIter __last,
	_BidirectionalIter __result,
	random_access_iterator_tag,
	_Distance*)
{
	for (_Distance __n = __last - __first; __n > 0; --__n)
		* --__result = *--__last;
	return __result;
}


template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
	return __copy_backward(__first, __last, __result,
		__ITERATOR_CATEGORY(__first),
		__DISTANCE_TYPE(__first));
}


template <class _InputIter, class _Size, class _OutputIter>
pair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count,
	_OutputIter __result,
	input_iterator_tag) {
	for (; __count > 0; --__count) {
		*__result = *__first;
		++__first;
		++__result;
	}
	return pair<_InputIter, _OutputIter>(__first, __result);
}

template <class _RAIter, class _Size, class _OutputIter>
inline pair<_RAIter, _OutputIter>
__copy_n(_RAIter __first, _Size __count,
	_OutputIter __result,
	random_access_iterator_tag) {
	_RAIter __last = __first + __count;
	return pair<_RAIter, _OutputIter>(__last, copy(__first, __last, __result));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
__copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
	return __copy_n(__first, __count, __result,
		__ITERATOR_CATEGORY(__first));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
	return __copy_n(__first, __count, __result);
}
template <class _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __value) {
	for (; __first != __last; ++__first)
		* __first = __value;
}

template <class _OutputIter, class _Size, class _Tp>
_OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value) {
	for (; __n > 0; --__n, ++__first)
		* __first = __value;
	return __first;
}

inline void fill(unsigned char* __first, unsigned char* __last,
	const unsigned char& __c) {
	unsigned char __tmp = __c;
	memset(__first, __tmp, __last - __first);
}

inline void fill(signed char* __first, signed char* __last,
	const signed char& __c) {
	signed char __tmp = __c;
	memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

inline void fill(char* __first, char* __last, const char& __c) {
	char __tmp = __c;
	memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}


template <class _InputIter1, class _InputIter2>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
	_InputIter1 __last1,
	_InputIter2 __first2) {
	while (__first1 != __last1 && *__first1 == *__first2) {
		++__first1;
		++__first2;
	}
	return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
	_InputIter1 __last1,
	_InputIter2 __first2,
	_BinaryPredicate __binary_pred) {
	while (__first1 != __last1 && __binary_pred(*__first1, *__first2)) {
		++__first1;
		++__first2;
	}
	return pair<_InputIter1, _InputIter2>(__first1, __first2);
}

template <class _InputIter1, class _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
	_InputIter2 __first2) {
	for (; __first1 != __last1; ++__first1, ++__first2)
		if (*__first1 != *__first2)
			return false;
	return true;
}

template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
	_InputIter2 __first2, _BinaryPredicate __binary_pred) {
	for (; __first1 != __last1; ++__first1, ++__first2)
		if (!__binary_pred(*__first1, *__first2))
			return false;
	return true;
}


template <class _InputIter1, class _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
	_InputIter2 __first2, _InputIter2 __last2) {
	for (; __first1 != __last1 && __first2 != __last2
		; ++__first1, ++__first2) {
		if (*__first1 < *__first2)
			return true;
		if (*__first2 < *__first1)
			return false;
	}
	return __first1 == __last1 && __first2 != __last2;
}

template <class _InputIter1, class _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
	_InputIter2 __first2, _InputIter2 __last2,
	_Compare __comp) {
	for (; __first1 != __last1 && __first2 != __last2
		; ++__first1, ++__first2) {
		if (__comp(*__first1, *__first2))
			return true;
		if (__comp(*__first2, *__first1))
			return false;
	}
	return __first1 == __last1 && __first2 != __last2;
}

inline bool
lexicographical_compare(const unsigned char* __first1,
	const unsigned char* __last1,
	const unsigned char* __first2,
	const unsigned char* __last2)
{
	const size_t __len1 = __last1 - __first1;
	const size_t __len2 = __last2 - __first2;
	const int __result = memcmp(__first1, __first2, min(__len1, __len2));
	return __result != 0 ? __result < 0 : __len1 < __len2;
}

inline bool lexicographical_compare(const char* __first1, const char* __last1,
	const char* __first2, const char* __last2)
{
	return lexicographical_compare((const signed char*)__first1,
		(const signed char*)__last1,
		(const signed char*)__first2,
		(const signed char*)__last2);
}


__STL_END_NAMESPACE
