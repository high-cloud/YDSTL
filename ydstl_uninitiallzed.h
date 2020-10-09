#ifndef YDSTL_UNINITIALIZED_H
#define YDSTL_UNINITIALIZED_H

#include"ydstl_iterator.h"
#include<type_traits>

// 主要包括三个函数：uninitialized_fill_n,uninitialized_fill,uninitialized_copy
// TODO:: 完成底层函数
namespace ydstl{



template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T&x, std::true_type)
{

}

template<typename ForwardIterator,typename Size,typename T,typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T&x,T1 *)
{
    typedef typename std::is_pod<T1>::value is_POD;
    return __uninitialized_fill_n_aux(first,Size,const T&x,is_POD);
}

template<typename InputIterator,typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result);


template<typename ForwardIterator,typename T>
void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x);


template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& x)
{
    return __uninitialized_fill_n(first,n,x,value_type(first));
}




}// end namespace ydstl

#endif