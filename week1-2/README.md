# 第1-2周 SGI STL的学习

> [作业相关](homework.md)

## STL简单概述

1. STL组成由以下6个部分组成
    1. 容器（Container）
    2. 空间配置器（allocator）
    3. 算法（Algorithm）
    4. 迭代器（Iterator）
    5. 仿函数（Function object）
    6. 适配器（Adaptor）

2. STL六大组件内在联系
    1. __空间分配器__ 为 __容器__ 分配内存空间。
       > 注意：无论是为什么类型分配内存，分配器都 __只是分配内存__，而不执行构造函数
    2. __算法__ 只关心具体的做法，而不关心是什么 __容器__ 。所以需要 __迭代器__ 作为 __容器__ 与 __算法__ 的桥梁，将两者连接起来。
    3. __算法__ 中会常常搭配 __仿函数__ 来更好的完成其工作
    4. __适配器__ 将一个class 的接口转换为另一个 class 的接口，使原本因接口不兼容而不能合作的 class，可以一起运作。
    5. __容器__ 所使用的 __迭代器__，其相当一部分操作是在 __容器__ 内实现的。

## 详细信息

1. 配置器
    1. 容器使用配置器来进行内存空间的 __分配__、 __释放__，其相关头文件为`stl_alloc.h`等。  
       空间分配器有两种:
        1. 第一级分配器`__malloc_alloc_template`，即时分配即时释放，
        2. 第二级分配器`__default_alloc_template`，小型内存池。
       这两种分配方式各有各的好坏。
    1. 容器中通过调用配置器的静态函数来 __分配__、__释放__ 内存，而配置器则在底层调用`malloc`和`free`来满足用户需求。

2. 迭代器
    1. 迭代器的本质就是"指针"，只不过这个指针有点特别，它比较智能。
    2. 算法通过迭代器来对容器进行操作，使算法不必了解容器本身的结构。
    3. 迭代器探幽
        1. 迭代器的相关实现 (源码位于`stl_iterator.h`和`stl_iterator_base.h`)

            ```cpp
            template <class _Category, class _Tp, class _Distance = ptrdiff_t,
                class _Pointer = _Tp *, class _Reference = _Tp &>
                struct iterator {
                typedef _Category  iterator_category;   // 迭代器的种类
                typedef _Tp        value_type;          // 迭代器所指对象的类型
                typedef _Distance  difference_type;     // 两两迭代器之间，距离的类型
                typedef _Pointer   pointer;             // 迭代器所指对象的指针
                typedef _Reference reference;           // 迭代器所指对象的引用
            };
            ```

        2. 迭代器又细分为以下几种类型

            ```cpp
            // 迭代器的tag。对应类型的迭代器中，其iterator_category就是对应的tag
            struct input_iterator_tag {};
            struct output_iterator_tag {};
            struct forward_iterator_tag : public input_iterator_tag {};
            struct bidirectional_iterator_tag : public forward_iterator_tag {};
            struct random_access_iterator_tag : public bidirectional_iterator_tag {};

            // 输入迭代器
            template <class _Tp, class _Distance> struct input_iterator {
                typedef input_iterator_tag iterator_category;
                typedef _Tp                value_type;
                typedef _Distance          difference_type;
                typedef _Tp* pointer;
                typedef _Tp& reference;
            };
            // 输出迭代器
            struct output_iterator {
                typedef output_iterator_tag iterator_category;
                typedef void                value_type;
                typedef void                difference_type;
                typedef void                pointer;
                typedef void                reference;
            };
            // 正向迭代器
            template <class _Tp, class _Distance> struct forward_iterator {
                typedef forward_iterator_tag iterator_category;
                typedef _Tp                  value_type;
                typedef _Distance            difference_type;
                typedef _Tp* pointer;
                typedef _Tp& reference;
            };

            // 双向迭代器
            template <class _Tp, class _Distance> struct bidirectional_iterator {
                typedef bidirectional_iterator_tag iterator_category;
                typedef _Tp                        value_type;
                typedef _Distance                  difference_type;
                typedef _Tp* pointer;
                typedef _Tp& reference;
            };
            // 随机访问迭代器
            template <class _Tp, class _Distance> struct random_access_iterator {
                typedef random_access_iterator_tag iterator_category;
                typedef _Tp                        value_type;
                typedef _Distance                  difference_type;
                typedef _Tp* pointer;
                typedef _Tp& reference;
            };
            ```

        3. 迭代器中的`iterator_category`和`value_type`等等，它们只是类型名。那么迭代器是如何将这些信息返回给调用者呢？  
            实际上，SGI STL灵活的使用了模板参数推导的特性来完成这些任务，例如如下代码

            ```cpp
            // iterator_category
            template <class _Tp, class _Distance>
            inline input_iterator_tag
            iterator_category(const input_iterator<_Tp, _Distance>&)
            {
                return input_iterator_tag();
            }

            inline output_iterator_tag iterator_category(const output_iterator&)
            {
                return output_iterator_tag();
            }
            #define __ITERATOR_CATEGORY(__i) iterator_category(__i)
            // value_type
            template <class _Tp, class _Distance>
            inline _Tp* value_type(const bidirectional_iterator<_Tp, _Distance>&)
            {
                return (_Tp*)(0);
            }

            template <class _Tp, class _Distance>
            inline _Tp* value_type(const random_access_iterator<_Tp, _Distance>&)
            {
                return (_Tp*)(0);
            }
            #define __VALUE_TYPE(__i)        value_type(__i)
            ```

            当不同类型的指针使用宏定义`__VALUE_TYPE`时，由于模板参数推导的特点，程序会调用参数类型相匹配的函数。既然参数类型匹配，那么返回的也一定是匹配的类型。
        4. `Traits`技术（重中之重）
            1. `iterator_traits`
                1. iterator_traits 技术用于萃取出iterator的对应类型，例如`value_type`、`iterator_category`等等。  
                   iterator本身便可以使用对应宏定义(例如`__VALUE_TYPE`)来取出对应类型。但原生指针并没有这些方法，所以需要套一层`iterator_traits`，让`iterator_traits`“帮助”原生指针，将所需的相关信息返回给调用者。
                2. `iterator_traits`相关源码

                   ```cpp
                   template <class _Iterator>
                   struct iterator_traits {
                   typedef typename _Iterator::iterator_category iterator_category;
                   typedef typename _Iterator::value_type        value_type;
                   typedef typename _Iterator::difference_type   difference_type;
                   typedef typename _Iterator::pointer           pointer;
                   typedef typename _Iterator::reference         reference;
                   };

                   template <class _Tp>
                   struct iterator_traits<_Tp*> {
                   typedef random_access_iterator_tag iterator_category;
                   typedef _Tp                         value_type;
                   typedef ptrdiff_t                   difference_type;
                   typedef _Tp*                        pointer;
                   typedef _Tp&                        reference;
                   };
                   ```

                   可以看到，`iterator_traits`对待iterator和原生指针的方式，是不一样的。
            2. `type_traits`
               1. `iterator_traits`技术只能用来规范迭代器，对于迭代器之外的东西没有加以规范。所以`type_traits`就应运而生。
               2. `iterator_traits`是萃取迭代器的特性，而`__type_traits`是萃取型别的特性。  
                  `__type_traits`有如下几个类型
                    1. `has_trivial_default_constructor` —— 是否使用默认构造函数
                    2. `has_trivial_copy_constructor` —— 是否使用默认拷贝构造函数
                    3. `has_trivial_assignment_operator` —— 是否使用默认赋值运算符
                    4. `has_trivial_destructor` —— 是否使用默认析构函数
                    5. `is_POD_type` —— 是否是`POD`类型
                  返回的是`__true_type`或`__false_type`结构
               3. 其相关源码如下

                    ```cpp
                    struct __true_type {};
                    struct __false_type {};

                    template <class _Tp>
                    struct __type_traits {
                        typedef __true_type     this_dummy_member_must_be_first;
                                        /* Do not remove this member. It informs a compiler which
                                            automatically specializes __type_traits that this
                                            __type_traits template is special. It just makes sure that
                                            things work if an implementation is using a template
                                            called __type_traits for something unrelated. */

                        /* The following restrictions should be observed for the sake of
                            compilers which automatically produce type specific specializations
                            of this class:
                                - You may reorder the members below if you wish
                                - You may remove any of the members below if you wish
                                - You must not rename members without making the corresponding
                                    name change in the compiler
                                - Members you add will be treated like regular members unless
                                    you add the appropriate support in the compiler. */

                        typedef __false_type    has_trivial_default_constructor;
                        typedef __false_type    has_trivial_copy_constructor;
                        typedef __false_type    has_trivial_assignment_operator;
                        typedef __false_type    has_trivial_destructor;
                        typedef __false_type    is_POD_type;
                    };

                    __STL_TEMPLATE_NULL struct __type_traits<int> {
                        typedef __true_type    has_trivial_default_constructor;
                        typedef __true_type    has_trivial_copy_constructor;
                        typedef __true_type    has_trivial_assignment_operator;
                        typedef __true_type    has_trivial_destructor;
                        typedef __true_type    is_POD_type;
                    };
                    ```

        5. 应用  
           为什么迭代器要分这么多的类型呢？原因是为了实现STL速度与效率的提高。  
           根据迭代器的类型，算法可以对该种类的迭代器使用效率最高的操作方式。  
           例如，如果对char*类型的iterator执行copy操作，那么copy函数就可以直接使用memcpy来完成操作，而不是遍历复制再构造。如此以提高算法的效率。
           请看如下源码：

            ```cpp
            vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x)
            {
                /* ....  */
                __STL_TRY{
                    // 当vector的成员函数调用 uninitialized_copy 时，程序会根据迭代器类型执行特定的 uninitialized_copy操作
                    __new_finish = uninitialized_copy(_M_start, __position, __new_start);
                    construct(__new_finish, __x);
                    ++__new_finish;
                    __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
                } __STL_UNWIND((destroy(__new_start, __new_finish),
                        _M_deallocate(__new_start, __len)));
                /* ....  */
            }

            // 如果是char*类型的迭代器就直接调用memmove。如果不是，则调用使用迭代器的uninitialized_copy函数
            template <class _InputIter, class _ForwardIter>
            inline _ForwardIter
            uninitialized_copy(_InputIter __first, _InputIter __last,
                _ForwardIter __result)
            {
                return __uninitialized_copy(__first, __last, __result,
                    // 注意到这里获取了迭代器所指向对象的类型
                    __VALUE_TYPE(__result));
            }

            // 如果迭代器是一个指向某个对象的指针，则调用内含_Tp*类型参数的__uninitialized_copy
            template <class _InputIter, class _ForwardIter, class _Tp>
            inline _ForwardIter
            __uninitialized_copy(_InputIter __first, _InputIter __last,
                                _ForwardIter __result, _Tp*)
            {
                // 注意这里获取了迭代器所指对象的is_POD信息
                typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
                return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
            }

            // 如果这个对象类型是POD的，直接copy以提高效率
            template <class _InputIter, class _ForwardIter>`
            inline _ForwardIter
            __uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                                    _ForwardIter __result,
                                    __true_type)
            {
                return copy(__first, __last, __result);
            }
            // 否则，只能一个个的遍历并执行构造函数
            template <class _InputIter, class _ForwardIter>
            _ForwardIter
            __uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                                    _ForwardIter __result,
                                    __false_type)
            {
                _ForwardIter __cur = __result;
                __STL_TRY {
                    for ( ; __first != __last; ++__first, ++__cur)
                    _Construct(&*__cur, *__first);
                    return __cur;
                }
                __STL_UNWIND(_Destroy(__result, __cur));
            }
            ```

3. 容器
    1. vector
        1. vector的本质就是一个空间连续数组。与普通数组不同的是，该数组"可长可短"。
        2. vector的核心成员是三个迭代器，分别为
           1. `_Tp* _M_start` —— 指向所分配数组的起始位置
           2. `_Tp* _M_finish` —— 指向已使用空间的末端位置+1
           3. `_Tp* _M_end_of_storage` —— 指向所分配数组的末尾位置+1
    2. list
       1. list是一个头尾相连的双向环状链表，由一个个节点头尾相连所构成。
       2. 其关键的成员只有一个，`_List_node<_Tp>* _M_node` —— 指向链表的末尾节点，该节点的成员`_M_next`指向的是链表的起始节点。
