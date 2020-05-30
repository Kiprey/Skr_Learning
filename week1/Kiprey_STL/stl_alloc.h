#pragma once
#include "stl_config.h"
#include "stl_construct.h"
#include <cstdlib>
#include <cstdio>


__STL_BEGIN_NAMESPACE


template <int __inst>
class __malloc_alloc_template {
public:

	static void* allocate(size_t __n)
	{
		void* __result = malloc(__n);
		if (0 == __result) {
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		return __result;
	}

	static void deallocate(void* __p, size_t /* __n */)
	{
		free(__p);
	}

	static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz)
	{
		void* __result = realloc(__p, __new_sz);
		if (0 == __result) {
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		return __result;
	}
};

typedef __malloc_alloc_template<0> malloc_alloc;

template<class _Tp, class _Alloc>
class simple_alloc {

public:
	static _Tp* allocate(size_t __n)
	{
		return 0 == __n ? 0 : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
	}
	static _Tp* allocate(void)
	{
		return (_Tp*)_Alloc::allocate(sizeof(_Tp));
	}
	static void deallocate(_Tp* __p, size_t __n)
	{
		if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(_Tp));
	}
	static void deallocate(_Tp* __p)
	{
		_Alloc::deallocate(__p, sizeof(_Tp));
	}
};
typedef malloc_alloc alloc;

__STL_END_NAMESPACE