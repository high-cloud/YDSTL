#ifndef __YDSTL_ALGOBASE_H__
#define __YDSTL_ALGOBASE_H__

#include<type_traits>
#include<string.h>
#include "ydstl_iterator.h"
// 基本算法

namespace ydstl
{

    //-----------------equal-----------------------------------------
    // 比较[first,last)是否一样
    template <typename InputIterator1, typename InputIterator2>
    bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
    {
        for (; first1 != last1; ++first1, ++first12)
        {
            if (*first1 != *first2)
                return false;
        }
        return true;
    }

    template <typename InputIterator1, typename InputIterator2, typename Compare>
    bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, Compare compare)
    {
        for (; first1 != last1; ++first1, ++first12)
        {
            if (!compare(*first1, *first2))
                return false;
        }
        return true;
    }

    //--------------------------------fill-----------------------------
    // 将[first,last)改填新值
    template <typename ForwardIterator, typename T>
    void fill(ForwardIterator first, ForwardIterator last, const T &value)
    {
        for (; first != last; ++first)
        {
            *first = value;
        }
    }

    //---------------fill_n----------------
    template <typename ForwardIterator, typename Size, typename T>
    ForwardIterator fill_n(ForwardIterator first, Size n, const T &value)
    {
        for (; n > 0; --n, ++first)
        {
            *first = value;
        }
        return first;
    }

    //--------------------------iter_swap--------------------
    // 交换两个iterneirong
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2)
    {
        __iter_swap(iter1, iter2, value_type(iter1));
    }

    template <typename ForwardIterator1, typename ForwardIterator2, typename T>
    inline void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2, T *)
    {
        T tmp = *iter1;
        *iter1 = *iter2;
        *iter2 = *iter1;
    }

    // ----------------------------------------字典排序----------------------------------
    //如果 第一个串小于或等于第二个串，返回true
    template <typename InputIterator1, typename InputIterator2>
    bool lexicongraphical_compare(InputIterator1 first1, InputIterator1 last1,
                                  InputIterator2 first2, InputIterator2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (*first1 < *first2)
            {
                return true;
            }
            else if (*first1 > *first2)
            {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    template <typename InputIterator1, typename InputIterator2, typename Compare>
    bool lexicongraphical_compare(InputIterator1 first1, InputIterator1 last1,
                                  InputIterator2 first2, InputIterator2 last2, Compare compare)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (compare(*first1, *first2))
            {
                return true;
            }
            else if (compare(*first2, *first1))
            {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    // ----------------------------max------------------------------
    //
    template <typename T>
    inline const T &max(const T &a, const T &b)
    {
        return a < b ? b : a;
    }

    template <typename T, typename Compare>
    inline const T &max(const T &a, const T &b, Compare comp)
    {
        return comp(a, b) ? b : a;
    }

    //--------------------------min----------------------------------
    //
    template <typename T>
    inline const T &min(const T &a, const T &b)
    {
        return a < b ? a : b;
    }

    template <typename T, typename Compare>
    inline const T &min(const T &a, const T &b, Compare comp)
    {
        return comp(a, b) ? a : b;
    }

    //--------------------------mismatch-------------------
    // 返回序列中第一个不同的迭代器，用户自行保证第二个序列不短于第一个序列
    //TODO (因为缺少 pair)

    //---------------------------swap------------------------------
    template <class T>
    inline void swap(T &a, T &b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    }

    //-----------------------copy-----------------------------
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }

    template <typename InputIterator, typename OutpuIterator>
    class __copy_dispatch
    {
        OutpuIterator operator()(InputIterator first, InputIterator last, OutpuIterator result)
        {
            return __copy(first, last, result, iterator_category(first));
        }
    };

    // 偏特化
    template <typename T>
    class __copy_dispatch<T*,T*>
    {
        T* operator(T* first, T* last, T* result)
        {
            typedef typename std::is_trivially_assignable<T> t;
            return __copy_t(first, last, result, t());
        }
    };

    // 偏特化
    template <typename T>
    class __copy_dispatch<const T*,T*>
    {
        T* operator(const T* first, const T* last, T* result)
        {
            typedef typename std::is_trivially_assignable<T> t;
            return __copy_t(first, last, result, t());
        }
    };

    template <typename InputIterator, typename OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
    {
        for(;first!=last;++first,++result)
        {
            *result=*first;
        }
        return result;
    }

    template <typename InputIterator, typename OutputIterator>
    OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag)
    {
        return __copy_d(first,last,result,distance_type(first));
    }

    template<typename RandowIterator,typename OutputIterator,typename T>
    OutputIterator __copy_d(RandowIterator first,RandowIterator last,OutputIterator result, T*)
    {
        for(T n=last-first;n!=0;++first,++result,--n)
        {
            *result=*first;
        }
        return result;
    }

    template<typename T>
    inline T* __copy_t(const T* first,const T* last , T* result, std::true_type)
    {
        memmove(result,first,sizeof(T)*(last-first));
        return result+(last-first);
    }

    template<typename T>
    inline T* __copy_t(const T* first,const T* last , T* result, std::false_type)
    {
        return __copy_d(first,last,result,T*);
    }

} // namespace ydstl

#endif